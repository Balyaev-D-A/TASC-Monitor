//#include <QtDebug>
#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    connect(ui->btnUpdate, &QToolButton::clicked, this, &ConnectionDialog::updatePorts);
    connect(ui->cbPort, &QComboBox::currentTextChanged, this, &ConnectionDialog::updateInfo);
    updatePorts();
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::updatePorts()
{
    QSerialPortInfo pi;
    QList<QSerialPortInfo> lpi;
    ui->cbPort->clear();
    lpi = QSerialPortInfo::availablePorts();
    foreach (pi, lpi) {
        ui->cbPort->addItem(pi.portName());
    }
}

bool ConnectionDialog::setPort(QString port)
{
    int idx;
    QSerialPortInfo pi;
    if (ui->cbPort->count() > 0) {
        idx = ui->cbPort->findText(port);
        if (idx != -1) {
            ui->cbPort->setCurrentIndex(idx);
            foreach (pi, QSerialPortInfo::availablePorts()) {
                if (pi.portName() == port)
                    if (!pi.isBusy()) return true;
            }
        }
        ui->cbPort->setCurrentIndex(0);
    }
    return false;
}

QString ConnectionDialog::getPort()
{
    return ui->cbPort->currentText();
}

void ConnectionDialog::updateInfo()
{
    bool idxfound = false;
    QSerialPortInfo pi;

    foreach (pi, QSerialPortInfo::availablePorts()) {
        if (pi.portName() == ui->cbPort->currentText()) {
            idxfound = true;
            break;
        }
    }
    if (idxfound) {
        if (pi.isBusy())
            ui->l_state->setText("Занят");
        else
            ui->l_state->setText("Свободен");
        ui->l_manufacturer->setText(pi.manufacturer());
        ui->l_serial->setText(pi.serialNumber());
        ui->l_location->setText(pi.systemLocation());
        ui->l_description->setText(pi.description());
    }
}
