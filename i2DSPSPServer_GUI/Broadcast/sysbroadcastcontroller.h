#ifndef SYSBROADCASTCONTROLLER_H
#define SYSBROADCASTCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QProcess>
#include "alsasoundobject.h"
#include "audiopcmoutput.h"

#define EMG_TALK_RECORD_PATH "sysEmgTalkRecordFile/"

class SysBroadcastController : public QObject
{
    Q_OBJECT
public:
    explicit SysBroadcastController(QObject *parent = nullptr);


signals:
    void sysPCEvents(QString cmdType , QVariant params = 0);
public slots:
    void userSetPCEvents(QString cmdType , QVariant params = 0);

private:
    AlsaSoundObject *m_pBroadcastObject;
    QProcess *m_pMplayerProcess;


};

#endif // SYSBROADCASTCONTROLLER_H
