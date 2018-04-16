#ifndef ALSASOUNDOBJECT_H
#define ALSASOUNDOBJECT_H

#include <QObject>
#include <QThread>
#include <alsa/asoundlib.h>

void playBackInit();
void captureInit();
void playBackInit();
void playOnePeriod(char *buffer , int len);

class AlsaCapthureThread : public QThread
{
public:
    explicit AlsaCapthureThread();
protected:
    void run();
};
class ALsaPlayBackThread : public QThread
{
public:
    explicit ALsaPlayBackThread();
protected:
    void run();
};

class AlsaSoundObject : public QObject
{
    Q_OBJECT
public:
    explicit AlsaSoundObject(QObject *parent = nullptr);

    void startAndStopBroadcat();

signals:

public slots:

private:
    AlsaCapthureThread *m_pCaptureThread;
    ALsaPlayBackThread *m_pPlayBackThread;
};

#endif // ALSASOUNDOBJECT_H
