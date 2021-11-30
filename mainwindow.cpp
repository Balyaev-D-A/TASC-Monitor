//#include <QtDebug>
#include <QtMultimedia/QSound>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    flowmeter = new FlowMeter(this);
    conDialog = new ConnectionDialog(this);
    comDialog = new CommonDialog(this);
    errDialog = new ErrorDialog(this);
    settings = new Settings(this);
    timer_update = new QTimer(this);
    timer_alarm = new QTimer(this);
    aboutDialog = new AboutDialog(this);
    lcdMenu = new QMenu(this);
    action_showAll = lcdMenu->addAction("Обычный режим");
    action_exit = lcdMenu->addAction("Выход");

    connect(ui->action_connect, &QAction::triggered, this, &MainWindow::showConnectDialog);
    connect(ui->action_about, &QAction::triggered, aboutDialog, &AboutDialog::show);
    connect(ui->action_common, &QAction::triggered, this, &MainWindow::showCommonDialog);
    connect(ui->action_trend, &QAction::toggled, this, &MainWindow::showTrend);
    connect(timer_update, &QTimer::timeout, this, &MainWindow::updateFlow);
    connect(ui->action_log, &QAction::triggered, errDialog, &ErrorDialog::show);
    connect(ui->trend->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(trendRangeChanged(QCPRange)));
    connect(ui->trend, &QCustomPlot::mouseDoubleClick, this, &MainWindow::trendDoubleClick);
    connect(timer_alarm, &QTimer::timeout, this, &MainWindow::timerAlarm);
    connect(ui->action_widget, &QAction::triggered, this, &MainWindow::onlyIndicator);
    connect(ui->lcdNumber, &QLCDNumber::customContextMenuRequested, this, &MainWindow::showLCDMenu);
    connect(ui->trend, &QLCDNumber::customContextMenuRequested, this, &MainWindow::showLCDMenu);
    connect(action_showAll, &QAction::triggered, this, &MainWindow::showAll);
    connect(action_exit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui->action_top, &QAction::toggled, this, &MainWindow::keepOnTop);

    comDialog->setInterval(settings->getInterval());
    comDialog->setHWarning(settings->getHWarning());
    comDialog->setLWarning(settings->getLWarning());
    comDialog->setHEmergency(settings->getHEmergency());
    comDialog->setLEmergency(settings->getLEmergency());
    comDialog->setAutoUnShutUp(settings->getAutoUnShutUp());
    comDialog->setSound(settings->getSound());
    ui->action_trend->setChecked(settings->getTrend());
    showTrend(settings->getTrend());
    restoreGeometry(settings->getGeometry());
    restoreState(settings->getState());
    ui->action_top->setChecked(settings->getOnTop());


    timer_update->setInterval(settings->getInterval() * 60000);

    QSharedPointer<QCPAxisTickerDateTime> ticker(new QCPAxisTickerDateTime);
    ticker->setDateTimeFormat("hh:mm");
    ticker->setTickStepStrategy(QCPAxisTicker::tssReadability);

    ui->trend->setInteraction(QCP::iRangeZoom, true);
    ui->trend->setInteraction(QCP::iRangeDrag, true);
    ui->trend->setBackground(QBrush(QColor(Qt::black)));
    ui->trend->xAxis->setTickLabelColor(QColor(Qt::white));
    ui->trend->yAxis->setTickLabelColor(QColor(Qt::white));
    ui->trend->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->trend->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->trend->xAxis->setTicker(ticker);
    ui->trend->addGraph();
    ui->trend->graph(0)->setPen(QPen(QBrush(QColor(Qt::blue)), 3));
    ui->trend->addGraph();
    ui->trend->graph(1)->setPen(QPen(QBrush(QColor(Qt::yellow)), 2, Qt::DotLine));
    ui->trend->addGraph();
    ui->trend->graph(2)->setPen(QPen(QBrush(QColor(Qt::red)), 2, Qt::DotLine));
    ui->trend->addGraph();
    ui->trend->graph(3)->setPen(QPen(QBrush(QColor(Qt::yellow)), 2, Qt::DotLine));
    ui->trend->addGraph();
    ui->trend->graph(4)->setPen(QPen(QBrush(QColor(Qt::red)), 2, Qt::DotLine));
    ui->trend->yAxis->setRange(settings->getLEmergency() - 10, settings->getHEmergency() + 10);
    lastTick = QDateTime::currentDateTime().toTime_t();
    ui->trend->xAxis->setRange(lastTick, settings->getInterval() * 50 * 60, Qt::AlignRight);

    ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: lightgrey}");
    ui->lcdNumber->display("---.--");
    if (settings->getWidgetState()) this->onlyIndicator();
    this->show();


    if (!conDialog->setPort(settings->getPort()))
    {
        QMessageBox::critical(this, "Ошибка", "Порт " + settings->getPort() + " не найден или занят.\nПожалуйста укажите другой порт для соединения.");
        return;
    }

    if (!flowmeter->serialConnect(settings->getPort()))
    {
        QMessageBox::critical(this, "Ошибка", "Порт " + settings->getPort() + ": " + flowmeter->getPortErrorString() + "\nПожалуйста укажите другой порт для соединения.");
        return;
    }

    updateFlow();
    timer_update->start();
}

MainWindow::~MainWindow()
{
    delete flowmeter;
    delete conDialog;
    delete comDialog;
    delete errDialog;
    delete settings;
    delete ui;
}

void MainWindow::showTrend(bool show)
{
    if (show)
        ui->lcdNumber->setMaximumHeight(95);
    else
        ui->lcdNumber->setMaximumHeight(16777215);

    ui->trend->setVisible(show);
    settings->setTrend(show);
}

void MainWindow::updateFlow()
{
    FlowResult fr;
    fr = flowmeter->requestFlow();
    if (!fr.dataSent) {
        ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: lightgrey}");
        errDialog->addError("Ошибка порта: " + flowmeter->getPortErrorString());
        ui->statusbar->showMessage("Ошибка порта: " + flowmeter->getPortErrorString());
        updateTrend();
        return;
    }

    if(!fr.dataRecieved) {
        ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: lightgrey}");
        errDialog->addError("Ошибка устройства: Устройство не отвечает");
        ui->statusbar->showMessage("Ошибка устройства: Устройство не отвечает");
        updateTrend();
        return;
    }

    if (fr.deviceError) {
        ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: lightgrey}");
        errDialog->addError("Ошибка устройства: " + flowmeter->deviceErrorCodeToString(fr.deviceError));
        ui->statusbar->showMessage("Ошибка устройства: " + flowmeter->deviceErrorCodeToString(fr.deviceError));
        updateTrend();
        return;
    }
    ui->statusbar->showMessage("Устройство работает нормально");
    ui->lcdNumber->display(QString::number(fr.flow, 'f', 2));
    processAlarms();
    updateTrend();
}

void MainWindow::unShutup()
{
    ui->action_alarm->setChecked(false);
}


void MainWindow::updateTrend()
{
    lastTick = QDateTime::currentDateTime().toTime_t();
    double value = ui->lcdNumber->value();
    ui->trend->graph(0)->addData(lastTick, value);
    ui->trend->graph(1)->addData(lastTick, settings->getHWarning());
    ui->trend->graph(2)->addData(lastTick, settings->getHEmergency());
    ui->trend->graph(3)->addData(lastTick, settings->getLWarning());
    ui->trend->graph(4)->addData(lastTick, settings->getLEmergency());

    if (ui->trend->xAxis->range().upper > lastTick - settings->getInterval() * 10 * 60)
        ui->trend->xAxis->setRange(lastTick, settings->getInterval() * 50 * 60, Qt::AlignRight);
    ui->trend->replot();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    flowmeter->serialDisconnect();
    settings->setState(saveState());
    settings->setGeometry(saveGeometry());
    settings->saveSettings();
    event->accept();
}

void MainWindow::showConnectDialog()
{
    timer_update->stop();
    flowmeter->serialDisconnect();
    conDialog->setPort(settings->getPort());
    if (conDialog->exec() == QDialog::Accepted) {
        if (conDialog->getPort() != "")
            settings->setPort(conDialog->getPort());
        else {
            QMessageBox::critical(this, "Ошибка", "Порт не выбран");
            ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: lightgrey}");
            ui->lcdNumber->display("---.--");
            return;
        }
    } else return;

        if (!flowmeter->serialConnect(settings->getPort()))
        {
            QMessageBox::critical(this, "Ошибка", "Порт " + settings->getPort() + ": " + flowmeter->getPortErrorString() + "\nПожалуйста укажите другой порт для соединения.");
            ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: lightgrey}");
            ui->lcdNumber->display("---.--");
            return;
        }
    updateFlow();
    timer_update->start();

}

void MainWindow::showCommonDialog()
{
    if (comDialog->exec() == QDialog::Accepted) {
        settings->setHWarning(comDialog->getHWarning());
        settings->setLWarning(comDialog->getLWarning());
        settings->setHEmergency(comDialog->getHEmergency());
        settings->setLEmergency(comDialog->getLEmergency());
        settings->setInterval(comDialog->getInterval());
        settings->setAutoUnShutUp(comDialog->getAutoUnShutUp());
        settings->setSound(comDialog->getSound());
        timer_update->stop();
        timer_update->setInterval(settings->getInterval() * 60000);
        timer_update->start();
        ui->trend->yAxis->setRange(settings->getLEmergency() - 10, settings->getHEmergency() + 10);
        ui->trend->replot();
    }
}

void MainWindow::trendRangeChanged(const QCPRange & newRange)
{
    ui->trend->xAxis->ticker().dynamicCast<QCPAxisTickerDateTime>()->setDateTimeFormat(((newRange.size() >= 86400) ||
                                                                                (newRange.lower < lastTick - 43200) ||
                                                                                (newRange.upper > lastTick + 43200)) ?
                                                                                    "dd MMM yy\nhh:mm" : "hh:mm");
    if (newRange.upper > lastTick)
        ui->trend->xAxis->setRange(lastTick, newRange.size(), Qt::AlignRight);
}

void MainWindow::trendDoubleClick(QMouseEvent *event)
{
    ui->trend->xAxis->setRange(lastTick, settings->getInterval() * 50 * 60, Qt::AlignRight);
    ui->trend->replot();
}

void MainWindow::processAlarms()
{
    double val = ui->lcdNumber->value();
    double lw = settings->getLWarning();
    double le = settings->getLEmergency();
    double hw = settings->getHWarning();
    double he = settings->getHEmergency();

    if (((val >= hw) && (val < he)) || ((val <= lw) && (val > le)))
        processWarning();
    else if ((val >= he) || (val <= le))
        processEmergency();
    else
        processNormal();
}

void MainWindow::processWarning()
{
    ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: yellow}");
    timer_alarm->setInterval(5000);
    timer_alarm->start();
    silent_count = 3;
}

void MainWindow::processEmergency()
{
    ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: #FF4500}");
    timer_alarm->setInterval(2000);
    timer_alarm->start();
    silent_count = 3;

}

void MainWindow::processNormal()
{
    ui->lcdNumber->setStyleSheet("QLCDNumber {background-color: #00FF7F}");
    if (timer_alarm->isActive())
        timer_alarm->stop();
    if (silent_count) silent_count--;
    if (!silent_count)
        if (settings->getAutoUnShutUp())
            ui->action_alarm->setChecked(false);
}

void MainWindow::timerAlarm()
{
    if (!settings->getSound()) return;
    if (!ui->action_alarm->isChecked()) {
        QString snd = QApplication::applicationDirPath() + "\\sounds\\";
        if (timer_alarm->interval() == 5000)
            snd.append("warning.wav");
        else
            snd.append("emergency.wav");
        QSound::play(snd);
    }
}

void MainWindow::onlyIndicator()
{
    ui->statusbar->hide();
    ui->menubar->hide();
    ui->lcdNumber->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->trend->setContextMenuPolicy(Qt::CustomContextMenu);
    hide();
    setWindowFlag(Qt::FramelessWindowHint);
    show();
    settings->setWidgetState(true);
}

void MainWindow::showLCDMenu(const QPoint &pos)
{
    lcdMenu->popup(ui->lcdNumber->mapToGlobal(pos));
}

void MainWindow::showTrendMenu(const QPoint &pos)
{
    lcdMenu->popup(ui->trend->mapToGlobal(pos));
}

void MainWindow::showAll()
{
    hide();
    setWindowFlag(Qt::FramelessWindowHint, false);
    ui->lcdNumber->setContextMenuPolicy(Qt::DefaultContextMenu);
    ui->trend->setContextMenuPolicy(Qt::DefaultContextMenu);
    ui->menubar->show();
    ui->statusbar->show();
    show();
    settings->setWidgetState(false);
}

void MainWindow::quit()
{
    close();
}

void MainWindow::keepOnTop(bool top)
{
    if (top) {
        hide();
        SetForegroundWindow(reinterpret_cast<HWND>(winId()));
        setWindowFlag(Qt::WindowStaysOnTopHint);
        show();
    } else {
        hide();
        setWindowFlag(Qt::WindowStaysOnTopHint, false);
        show();
    }
    settings->setOnTop(top);
}
