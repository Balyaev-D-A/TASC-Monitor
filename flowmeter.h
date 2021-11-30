#ifndef FLOWMETER_H
#define FLOWMETER_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

#define MAX_FLOW 250

typedef struct {
    float flow;
    uchar deviceError;
    bool dataSent;
    bool dataRecieved;
} FlowResult;

class FlowMeter : public QObject
{
    Q_OBJECT
public:
    explicit FlowMeter(QObject *parent = nullptr);
    bool serialConnect(QString portname);
    void serialDisconnect();
    QString deviceErrorCodeToString(uchar code);
    QString getPortErrorString();
    FlowResult requestFlow();

private:
    QSerialPort *port;
};

#endif // FLOWMETER_H
