#ifndef AUDIOPCMOUTPUT_H
#define AUDIOPCMOUTPUT_H

#include <QObject>
#include <QFile>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QTimer>
#include <QDebug>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QNetworkDatagram>

class AudioPCMOutPut : public QObject
{
    Q_OBJECT
public:
    explicit AudioPCMOutPut(QObject *parent = nullptr);
    void playOrStopPCMAudioFile(bool isPlay ,QString fileName);
    void isPausePCMAudioFile(bool isPause);
    void playSPKTestFile();

signals:
    void  fileOutputOver();
public slots:

private:
    QTimer writeDataTimer;
    void write_pcm();

   QFile m_fAudioFile;
   QFile buffer;

   QAudioDeviceInfo m_AudioDeviceInfo;
   QAudioOutput *m_pAudioOutPut;
   QAudioOutput *m_pRecordFileOutPut;
   QIODevice *m_pIO;

   QUdpSocket m_UdpSendSocket;
   QAudioFormat audioFormat;


};

#endif // AUDIOPCMOUTPUT_H
