#include <QtDebug>
#include "flowmeter.h"

FlowMeter::FlowMeter(QObject *parent) : QObject(parent)
{
    port = new QSerialPort(this);
}

bool FlowMeter::serialConnect(QString portname)
{
    port->setPortName(portname);
    port->setBaudRate(QSerialPort::Baud57600);
    port->setDataBits(QSerialPort::Data8);
    port->setStopBits(QSerialPort::OneStop);
    port->setParity(QSerialPort::OddParity);
    port->setFlowControl(QSerialPort::NoFlowControl);

    if (port->open(QSerialPort::ReadWrite))
        return true;
    else
        return false;

}

void FlowMeter::serialDisconnect()
{
    if (port != nullptr)
        if (port->isOpen())
            port->close();

}

FlowResult FlowMeter::requestFlow()
{
    QByteArray data;
    uchar response;
    uchar msb;
    uchar lsb;
    uchar crc;
    short int flowdata;
    FlowResult fr;
    char cmd;
    short int maxflow;

    fr.flow = 0.0;
    fr.deviceError = 0;

    cmd = 0x73;
    port->write(&cmd, 1);
    fr.dataSent = port->waitForBytesWritten(100);
    fr.dataRecieved = port->waitForReadyRead(500);
  //  qDebug() << fr.dataSent << fr.dataRecieved;
    if (!fr.dataSent) return fr;
    if (!fr.dataRecieved) return fr;
    data = port->readAll();
    qDebug() << "Data recieved: " << data;
    response = data[0];
    if (response != 0x31) { // если ответ не на мой запрос
        if (response == 0x45){  // если ошибка
            fr.deviceError = data[1];
            return fr;
        }
        return fr;
    }
    crc = data[18];
    msb = data[3];
    lsb = data[4];
 //   if (crc != static_cast<uchar>(data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7] + data[8] + data[9] + data[10] + data[11] + data[12] + data[13] + data[14] + data[15] + data[16] + data[17])) {
 //       fr.deviceError = 0xe;  // Ошибка CRC в принятых данных
 //       return fr;
 //   }
    maxflow = (msb << 8) + lsb;
    qDebug() << "Max flow = " << maxflow;

   // qDebug() << "Requesting flow";
    cmd = 0x31;
    port->write(&cmd, 1);
    fr.dataSent = port->waitForBytesWritten(100);
    fr.dataRecieved = port->waitForReadyRead(500);
  //  qDebug() << fr.dataSent << fr.dataRecieved;
    if (!fr.dataSent) return fr;
    if (!fr.dataRecieved) return fr;


    data = port->readAll();
  //  qDebug() << "Data recieved: " << data;
    response = data[0];
    if (response != 0x31) { // если ответ не на мой запрос
        if (response == 0x45){  // если ошибка
            fr.deviceError = data[1];
            return fr;
        }
        return fr;
    }

    msb = data[1];
    lsb = data[2];
    crc = data[3];
    QString s;


    if (crc != static_cast<uchar>(response + msb + lsb)) {
        fr.deviceError = 0xe;  // Ошибка CRC в принятых данных
        return fr;
    }

    flowdata = (msb << 8) + lsb;

    s.sprintf("%x, %x, %x, %x, %x, %i", response, msb, lsb, crc, flowdata, flowdata);
    qDebug() << s;

    fr.flow = ((flowdata * maxflow) / 10000);
    qDebug() << fr.flow;
    return fr;

}


QString FlowMeter::deviceErrorCodeToString(uchar code)
{
    QString result;

    switch (code) {
    case 0x01:
        return "Возникла внутренняя ошибка, устройство не может ответить на запрос\n";

    case 0x02:  // /////////////////////////////////////////////////////////////////////////
    case 0x04:  //
    case 0x06:  //
    case 0x08:  //
    case 0x10:  //  Ошибки UART. Могут возникать кучкой. Определятся битовой маской
    case 0x12:  //
    case 0x14:  //
    case 0x16:  //
    case 0x18:  //
    case 0x20:  // /////////////////////////////////////////////////////////////////////////
    {
        result = "Возникли следующие ошибки UART:\n";
        if (code & 0x02) result += "\tУстройство занято обработкой предыдущего запроса.\n";
        if (code & 0x04) result += "\tУстройство не может прочитать данные\n";
        if (code & 0x08) result += "\tОтсутствует стоп-бит\n";
        if (code & 0x10) result += "\tКонтроль четности не пройден\n";
        if (code & 0x20) result += "\tОтсутствует стартовый бит\n";
        return result;
    }
    case 0x40:
        return "Неверный запрос\n";
    case 0x50:
        return "Ошибка самотестирования. Устройство неисправно\n";
    case 0x60:
        return "Ошибка инициализации данных EEPROM. Устройство неисправно\n";
    case 0xc0:
        return "Неизвестный идентификатор переменной\n";
    case 0xd0:
        return "Устройство не отвечает\n";
    case 0xe:
        return "Ошибка CRC в принятых данных\n";

    }
    return "Неизвестная ошибка\n";
}

QString FlowMeter::getPortErrorString()
{
    return port->errorString();
}
