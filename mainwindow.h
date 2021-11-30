#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
#include "connectiondialog.h"
#include "commondialog.h"
#include "settings.h"
#include "qcustomplot.h"
#include "flowmeter.h"
#include "errordialog.h"
#include "aboutdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void portError(QSerialPort::SerialPortError err);
    void deviceError(uchar err);

public slots:
    void showTrend(bool show);
    void updateFlow();
    void unShutup();
    void showConnectDialog();
    void showCommonDialog();
    void trendRangeChanged(const QCPRange &newRange);
    void trendDoubleClick(QMouseEvent *event);
    void timerAlarm();
    void onlyIndicator();
    void showLCDMenu(const QPoint &pos);
    void showTrendMenu(const QPoint &pos);
    void showAll();
    void quit();
    void keepOnTop(bool top);

private:
    Ui::MainWindow *ui;
    CommonDialog *comDialog;
    ConnectionDialog *conDialog;
    ErrorDialog *errDialog;
    AboutDialog *aboutDialog;
    Settings *settings;
    FlowMeter *flowmeter;
    QTimer *timer_update;
    QTimer *timer_alarm;
    char silent_count;
    uint lastTick;
    QWidget *lcdParent;
    QRect lcdGeometry;
    QMenu *lcdMenu;
    QAction *action_showAll;
    QAction *action_exit;
    void updateTrend();
    void processAlarms();
    void processWarning();
    void processEmergency();
    void processNormal();

protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
