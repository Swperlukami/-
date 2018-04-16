#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QObject>
#include "ShareMemory/queuesharememory.h"
#include "BuildingMapModel/buildingmapmodel.h"
#include <QQmlApplicationEngine>
//#include "sdcontroller.h"
#include "SPController/spcontroller.h"
#include "APController/apcontroller.h"

#include "EventControllers/channelmusicevent.h"
#include "EventControllers/audiofilesmanagecontroller.h"
#include "Broadcast/sysbroadcastcontroller.h"
#include <QFileSystemModel>
#include <QDirModel>


struct EventControllers{
    ChannelMusicEvent *pChannelMusicController = NULL;
    AudioFilesManageController *pAudioFileManageController = NULL;
    SysBroadcastController *pSysBroadcastController = NULL;
};

class UIController : public QObject
{
    Q_OBJECT
public:
    explicit UIController(QObject *parent = nullptr);

    EventControllers m_EventControllers;

signals:

public slots:
private:
    QQmlApplicationEngine engine;
    QueueShareMemory *mpGuiSHM;




//    SDController *mp_SDController;
    SPController *mp_SPController;
    APController *mp_APController;

    QTimer mt_ReadSHMTimer;
    QQueue<QByteArray>mq_SHMData;
    void readSHMTimerEvent();

};

#endif // UICONTROLLER_H
