#include "audiopcmoutput.h"
#include <QByteArray>

AudioPCMOutPut::AudioPCMOutPut(QObject *parent) : QObject(parent)
{
    audioFormat.setCodec("audio/pcm");
    audioFormat.setChannelCount(1);
    audioFormat.setSampleRate(8000);
    audioFormat.setSampleSize(8);
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::UnSignedInt);



    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(audioFormat)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }


    m_pAudioOutPut = new QAudioOutput(audioFormat ,this);
    m_pAudioOutPut->setVolume(0.2);
    //    connect(m_pAudioOutPut , &QAudioOutput::notify , this ,&AudioOutPut::notified);
//        m_pAudioOutPut->start(&m_fAudioFile);

//    audioFormat.setChannelCount(2);
//    audioFormat.setSampleRate(44100);
//    audioFormat.setSampleSize(16);


    if (!info.isFormatSupported(audioFormat)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    m_pRecordFileOutPut = new QAudioOutput(audioFormat , this);
    m_pIO = m_pRecordFileOutPut->start();
    m_pRecordFileOutPut->setVolume(1);

    connect(&writeDataTimer , &QTimer::timeout , this ,&AudioPCMOutPut::write_pcm);

    writeDataTimer.setInterval(100);

//    connect(m_pAudioOutPut , &QAudioOutput::stateChanged , [&](){
//        if(m_pAudioOutPut->state() == QAudio::IdleState){
//            writeDataTimer.stop();
//            m_fAudioFile.close();
//        }
//    });

//    playSPKTestFile();

//    m_UdpSendSocket.setParent(this);
//    if(!m_UdpSendSocket.bind(QHostAddress("224.1.1.1") , 5006 , QAbstractSocket::ShareAddress))
//        qDebug()<<m_UdpSendSocket.error();
//    else
//        qDebug()<<"Udp Send Socket Bind OK";

//    m_UdpSendSocket.setMulticastInterface(QNetworkInterface::allInterfaces().at(1));
//    qDebug()<<m_UdpSendSocket.error();

//    m_UdpSendSocket.setSocketOption(QAbstractSocket::MulticastTtlOption , 1);
}
void AudioPCMOutPut::playOrStopPCMAudioFile(bool isPlay, QString fileName)
{
    if(isPlay){
        if(writeDataTimer.isActive())
            writeDataTimer.stop();

        if(m_fAudioFile.isOpen())
            m_fAudioFile.close();
        m_fAudioFile.setFileName(fileName);

        if(!m_fAudioFile.open(QFile::ReadOnly)){
            qDebug()<<"PCM File Open Error";
            return;
        }
        qDebug()<<m_pRecordFileOutPut->format();
        m_pRecordFileOutPut->start(&m_fAudioFile);
//        writeDataTimer.start();
    }
    else{
        if(m_fAudioFile.isOpen())
            m_fAudioFile.close();

        writeDataTimer.stop();
    }
}
void AudioPCMOutPut::isPausePCMAudioFile(bool isPause)
{
    if(isPause)
        writeDataTimer.stop();
    else
        writeDataTimer.start();

}
void AudioPCMOutPut::playSPKTestFile()
{
    m_fAudioFile.setFileName("./Audio/sysAudio/Voice.wav");
//    m_fAudioFile.setFileName("./Audio/sysBroadcastRecordFile/18-01-18 11:52:58.pcm");

    if(!m_fAudioFile.open(QFile::ReadOnly)){
        qDebug()<<"PCM File Open Error";
        return;
    }
//    QByteArray tmp = m_fAudioFile.readAll();
//    qDebug()<<tmp.size();
//    tmp = tmp.mid(500, tmp.size()-7000);
//    buffer.setFileName("./Audio/sysAudio/Voice.wav");
//    buffer.open(QFile::ReadWrite | QFile::Truncate);
//    buffer.write(tmp);
//    buffer.close();
//    buffer.open(QFile::ReadOnly);
//    m_pAudioOutPut->start(&buffer);

    m_pAudioOutPut->start(&m_fAudioFile);
//    m_pRecordFileOutPut->start(&m_fAudioFile);
//    playOrStopPCMAudioFile(true , "./Audio/sysAudio/VoiceTest.wav");
}
void AudioPCMOutPut::write_pcm()
{
    QByteArray buf;
//    if(m_fAudioFile.isOpen() && m_UdpSendSocket.open(QIODevice::WriteOnly)){
//        buf = m_fAudioFile.read(1026);
//        if(m_UdpSendSocket.writeDatagram(buf.data(), buf.size(),QHostAddress("224.1.1.1"),5006) != buf.length()){
//            qDebug()<<"Send Record Audio Data Error";
//            qDebug()<<m_UdpSendSocket.errorString();
//        }
//        else
//            qDebug()<<"Send UDP Data";
//        if(m_fAudioFile.atEnd()){
//            emit fileOutputOver();
//            writeDataTimer.stop();
//            m_fAudioFile.close();
//        }
//    }

    qDebug()<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa";

    int fileSize = m_fAudioFile.size();
    static int sendSize = 0 ;
    if(m_fAudioFile.isOpen() && m_pRecordFileOutPut && m_pRecordFileOutPut->state() != QAudio::StoppedState)
    {
        int chunks = m_pRecordFileOutPut->bytesFree()/m_pRecordFileOutPut->periodSize();
        qDebug()<<"chunks :"<<chunks;
        while (chunks) {
            if(!m_fAudioFile.isOpen())
                break;

                const qint64 len = m_fAudioFile.read(buf.data() ,m_pRecordFileOutPut->periodSize());
//            qDebug()<<"Play Record Data:"<<buf;
            if (len){
                int size = m_pIO->write(buf.data() , len);
                if(size != len)
                    qDebug()<<"Write IO Audio Data Error";
                else
                    sendSize += size;
            }
            if (len != m_pRecordFileOutPut->periodSize())
                break;
            --chunks;
        }

        if(sendSize >= fileSize){
            sendSize = 0 ;
            emit fileOutputOver();
            writeDataTimer.stop();
            m_fAudioFile.close();
        }
    }
    else
        writeDataTimer.stop();

}
