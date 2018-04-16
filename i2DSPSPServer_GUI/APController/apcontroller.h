#ifndef APCONTROLLER_H
#define APCONTROLLER_H

#include <QObject>
#include <QHash>
#include <QByteArray>
#include <QDir>
#include <QSound>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaService>
#include <QProcess>
#include <QTimer>
#include <QDateTime>
#include <QUdpSocket>
#include "SysDataBase/database.h"
#include "SysProrocol/server_ui_protocol.h"

//#include "Broadcast/alsasoundobject.h"
//#include "Broadcast/audiopcmoutput.h"
//#include "UDPRecordFileClient/udpboradcastclient.h"

class APController : public QObject
{
    Q_OBJECT
public:
    explicit APController(QObject *parent = nullptr);

    void initAPDevices();

signals:
    void apEvents(QString cmdType , QVariant params = 0);
    void initAPInfo(QString apip , QString martixip , int inputno , int apstate , QString addr , QString apname);


public slots:
    void gotServerAPEvent(QByteArray apEventBuffer);
    void userSetAPEvents(QString cmdType , QVariant parmas = 0);
private:


    void sys_device_info_record(QString msg);
};

#endif // APCONTROLLER_H
