#ifndef ALSASOUNDOBJECT_H
#define ALSASOUNDOBJECT_H

#include <QObject>
#include <QThread>
#include <alsa/asoundlib.h>

void captureInit();
void stopCapture();
void playBackInit();
void stopPlatBack();
void playOnePeriod(char *buffer , int len);

class LoadFileThread : public QThread
{
public:
    explicit LoadFileThread(QByteArray recordData){
        m_RecordData = recordData;
    }

protected:
    void run();
private:
    QByteArray m_RecordData;

};


class AlsaCapthureThread : public QThread
{
public:
    explicit AlsaCapthureThread();
    ~AlsaCapthureThread();
protected:
    void run();
};
class ALsaPlayBackThread : public QThread
{
public:
    explicit ALsaPlayBackThread();
    ~ALsaPlayBackThread();
protected:
    void run();
};

class AlsaSoundObject : public QObject
{
    Q_OBJECT
public:
    explicit AlsaSoundObject(QObject *parent = nullptr);

    void startAndStopBroadcat(bool isBroadcat, bool isRecord = false, QString recordPath ="");
    void replayBroadcastFile(QString recordFileName, bool isReplay);
signals:

public slots:

private:
    AlsaCapthureThread *m_pCaptureThread;
    ALsaPlayBackThread *m_pPlayBackThread;
    LoadFileThread *m_pLoadRcordFileThread;
};




#endif // ALSASOUNDOBJECT_H
