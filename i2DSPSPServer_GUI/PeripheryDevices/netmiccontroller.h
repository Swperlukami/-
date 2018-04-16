#ifndef NETMICCONTROLLER_H
#define NETMICCONTROLLER_H

#include <QObject>
#include "tcpsocketbasecontroller.h"
#include "SysDataBase/database.h"
#include "periphery_protocol.h"


class NetMicController : public TcpSocketBaseController
{
    Q_OBJECT
public:
    explicit NetMicController(QObject *parent = nullptr);

signals:
    void netMicEvents(QString cmdType , QVariant params = 0);

public slots:
    void userSetNetMicEvent(QString cmdType , int voice = 0);

private slots:
    void netMicConnected();
    void netMicDisConnected();

private:
    void processData(QByteArray data);
    void sendNetMicData(bool isBro, bool isEmg, bool isDeviceError, bool isVoice);

};

#endif // NETMICCONTROLLER_H
