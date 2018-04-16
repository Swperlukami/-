#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QTimer>
#include <QDebug>
#define FLINE __FILE__<<"Line:"<<__LINE__


class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QString serialPortName , QObject *parent = nullptr);
    ~SerialPort();

    QSerialPort *m_pSerialPort;

    void setSerialPortFormat(QSerialPort::BaudRate baudRate , QSerialPort::DataBits data , QSerialPort::Parity parity , QSerialPort::StopBits stopBit);
    void wirteSerialPort(QByteArray data , int len);

signals:
    void powerEvent(int type , int powerevent);
    void btnEvent(QString event);
public slots:
    void setSerialPortName(QString portName);
    void readSerialData();

private:
    QString m_strSerialPortName;
    QTimer m_tSerialPortCheckTimerl;

    bool m_isOpenSerialPort;

    void checkSerialPortOK();
    void openRightSerialPort();
protected:
    virtual void processSerialData(QByteArray data) = 0 ;


};

#endif // SERIALPORT_H
