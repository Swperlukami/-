#include "sysbroadcastcontroller.h"
#include <QApplication>
#include "uicontroller.h"
#include "PeripheryDevices/powermodeserver.h"

extern PowerModeServer *g_pPowerSerialPort;
extern UIController *g_pUIController;

SysBroadcastController::SysBroadcastController(QObject *parent) : QObject(parent)
{
    m_pBroadcastObject = new AlsaSoundObject(this);
    m_pMplayerProcess = new QProcess(this);
}
void SysBroadcastController::userSetPCEvents(QString cmdType, QVariant params)
{
    if(cmdType == "Emg"){
        QString emgFile = QApplication::applicationDirPath() + "/Audio/sysAudio/"+params.toString();
        QString cmdStr = QString("mplayer -slave -loop 0 -volume 80 \"%1\" ").arg(emgFile);
        m_pMplayerProcess->start(cmdStr);
    }
    else if(cmdType == "CancelEmg"){
        m_pMplayerProcess->kill();
        m_pMplayerProcess->waitForFinished(2000);
    }
    else if(cmdType == "EmgTalk"){
        m_pBroadcastObject->startAndStopBroadcat(true , true , EMG_TALK_RECORD_PATH);
    }
    else if(cmdType == "StoplEmgTalk"){
        m_pBroadcastObject->startAndStopBroadcat(false);
        //Syn Emg Record Files
        g_pUIController->m_EventControllers.pAudioFileManageController->initSysEmgRecordFiles();
    }
    else if(cmdType == "Talk"){
        m_pBroadcastObject->startAndStopBroadcat(true);
    }
    else if(cmdType == "StopTalk"){
        m_pBroadcastObject->startAndStopBroadcat(false);
    }
    else if(cmdType == "ReplayPCMFile"){
        m_pBroadcastObject->replayBroadcastFile(params.toString() , true);
    }
    else if(cmdType == "StopReplayPCMFile"){
        m_pBroadcastObject->replayBroadcastFile(params.toString() , false);
    }
}
