#ifndef RS485_H
#define RS485_H
#include "usc4000_protocol.h"
#include "serialport.h"
#include <QObject>

class RS485 : public SerialPort
{
    Q_OBJECT
public:
    explicit RS485(QString serialPortName , QObject *parent = 0);
    ~RS485();
signals:
    void rs485Event(QString event);

protected:
    void processSerialData(QByteArray data);

private:
    bool setSerialPortParity(QSerialPort::Parity parity);

    unsigned char rs485AddrCode ;
    QByteArray Buffer;
    bool isGotAddrCode = false ;
    bool isGotSynCode = false ;


    QTimer m_tClearErrorBufferTimer;


    void sendUReplyPolling();
    void handelUSC4000FireInfo(USC4000_FIRE_INFO fireInfo);
};

#endif // RS485_H
