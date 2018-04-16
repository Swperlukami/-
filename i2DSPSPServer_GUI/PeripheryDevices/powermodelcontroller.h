#ifndef POWERMODELCONTROLLER_H
#define POWERMODELCONTROLLER_H

#include <QObject>
#include "SysDataBase/database.h"
#include "PeripheryDevices/periphery_protocol.h"
#include "tcpsocketbasecontroller.h"


class PowerModelController : public TcpSocketBaseController
{
    Q_OBJECT
public:
    explicit PowerModelController(QObject *parent = nullptr);

signals:
    void powerModelEvents(QString eventType , QVariant params = 0);

public slots:
    void userSetPowerModelEvent(bool isBro , bool isEmg);

    void sysSetPowerModelEVent(bool devError , bool isVoice);
    void sysSetPowerModelSelfTest();
private slots:
    void powerConnected();
    void powerDisconnected();

private:
    void processData(QByteArray data);
    void sendPowerModelData(bool isBro , bool isEmg , bool isDeviceError , bool isVoice);

    bool isSysBro ;
    bool isSysEmg ;
    bool isSysDevError ;
    bool isSysVoice ;
};

#endif // POWERMODELCONTROLLER_H
