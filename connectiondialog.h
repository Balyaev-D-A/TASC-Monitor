#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();
    bool setPort(QString port);
    QString getPort();

public slots:
    void updatePorts();
    void updateInfo();

private:
    Ui::ConnectionDialog *ui;
    QList<QSerialPortInfo> available;
    QSerialPortInfo *pInfo;
    QStringList ports;
};

#endif // CONNECTIONDIALOG_H
