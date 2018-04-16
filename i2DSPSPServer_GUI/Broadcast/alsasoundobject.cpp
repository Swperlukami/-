#include "alsasoundobject.h"
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <QQueue>
#include <QDebug>
#include <QDataStream>
#include <QMutex>
#include <QFile>
#include <QDateTime>
//#include <QtConcurrent>

QFile recordFile;
FILE *fp = NULL;
QByteArray replayRecordDataBuffer;
QDataStream recordDataBuffer;
QMutex mutex;

QQueue<QByteArray>dataQueue;

char *m_pCaptureBuffer;
snd_pcm_uframes_t m_periodSize;
snd_pcm_uframes_t m_frames = 300;
snd_pcm_t *m_pCaptureHnadel;
snd_pcm_t *m_pPlayBackHandel;
int m_channels = 1;
unsigned int m_sampleSize = SND_PCM_FORMAT_S16;
//unsigned int m_sampleSize = 16;
unsigned int m_sampleRate = 44100;
snd_pcm_hw_params_t *m_phw_capture_params;
snd_pcm_hw_params_t *m_phw_playbback_params;

bool isBroadcastThreadRunning = false ;
bool isReplayRecordFileRunning = false ;

int sendReciordSize = 0;
int recordFileSize = 0 ;
int playRecordSize = 0 ;

void readOnePeriod()
{
    int ret=0;
    memset(m_pCaptureBuffer , 0 , m_periodSize);
    ret = snd_pcm_readi(m_pCaptureHnadel, m_pCaptureBuffer, m_frames);
    //    printf("Capture read frame size: %d buffer size:%d \n" , ret , m_periodSize);

    //    qDebug()<<"********************* read Record Data:";
    //    ret = recordFile.read(m_pCaptureBuffer , m_periodSize);
    //    ret = tmpBuffer.readRawData(m_pCaptureBuffer , m_periodSize);

    if (ret == -EPIPE)
    {
        printf("OOOOO   overrun occurred\n");
        snd_pcm_prepare(m_pCaptureHnadel);
    }
    else if (ret <0)
    {
        printf("error from read\n");
    }
    else if (ret != m_frames)
    {
        printf("short read, read %d frames\n",ret);
    }


}

void playOnePeriod(char *buffer, int len)
{
    int ret = 0;
    int dir = 0;
    snd_pcm_uframes_t val = 0;

    //    printf("playback frame %d \n", m_frames);
    //    snd_pcm_hw_params_get_period_size(m_phw_params ,&m_frames , &dir);
    snd_pcm_hw_params_get_buffer_size(m_phw_playbback_params , &val);

    //    printf("playback got frame size %d \n", m_frames);
    //    printf("playback got buffer size %d \n", val);


    if(len != m_frames)
    {
        printf("snd_pcm_writei frames error!\n");
    }

    ret = snd_pcm_writei(m_pPlayBackHandel , buffer , m_frames);  //peroid_frame

    if(recordFile.isWritable()){
        //        tmpBuffer.append(buffer , m_periodSize);
        //        tmpBuffer.writeBytes(buffer , m_periodSize);
        recordDataBuffer.writeRawData(buffer , m_periodSize);
        //        if(recordFile.write(buffer , (int)m_periodSize) != m_periodSize){
        //            qDebug()<<"write record file error";
        //        }
        //        else{
        //            //        qDebug()<<"write record file OKOK";
        //        }
    }


    if (ret == -EPIPE)
    {
        printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU playback under run \n");

        snd_pcm_prepare(m_pPlayBackHandel);

        usleep(15000);
    }
    else if(ret <0)
    {
        printf("error from writei");
    }
    else if(ret != m_frames){
        printf("short  write : %d" , ret);
    }

}
void stopCapture()
{
    free(m_pCaptureBuffer);
    snd_pcm_drain(m_pCaptureHnadel);
    snd_pcm_close(m_pCaptureHnadel);
    qDebug()<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Stop Play Capture";

}
void stopPlatBack()
{
    snd_pcm_drain(m_pPlayBackHandel);
    snd_pcm_close(m_pPlayBackHandel);
    qDebug()<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Stop Play Back";
}

void captureInit()
{
    int rc;
    int dir;

    rc = snd_pcm_open(&m_pCaptureHnadel, "default",SND_PCM_STREAM_CAPTURE, 0);
    if (rc <0)
    {
        printf("unable to open capture pcm device!\n");
        exit(1);
    }

    snd_pcm_hw_params_malloc(&m_phw_capture_params);
    rc = snd_pcm_hw_params_any(m_pCaptureHnadel, m_phw_capture_params);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_any error\n");
    }

    rc = snd_pcm_hw_params_set_access(m_pCaptureHnadel, m_phw_capture_params , SND_PCM_ACCESS_RW_INTERLEAVED);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_access error\n");
    }

    rc = snd_pcm_hw_params_set_format(m_pCaptureHnadel, m_phw_capture_params , (snd_pcm_format_t)m_sampleSize);

    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_format error\n");
    }

    rc = snd_pcm_hw_params_set_channels(m_pCaptureHnadel, m_phw_capture_params, m_channels);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_channels error\n");
    }

    rc = snd_pcm_hw_params_set_rate_near(m_pCaptureHnadel,m_phw_capture_params , &m_sampleRate , &dir);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_rate_near error\n");
    }

    // set period size 设置周期大小 ,以帧来计算的
    //    m_frames = 128 ;
    rc = snd_pcm_hw_params_set_period_size_near(m_pCaptureHnadel , m_phw_capture_params, &m_frames, &dir);
    printf("init  capture period frame size: %d \n" , m_frames);

    //    m_periodSize = (snd_pcm_uframes_t)m_sampleSize*m_channels*m_frames;
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_peroid_size_near error\n");
    }

    snd_pcm_hw_params_get_period_size(m_phw_capture_params , &m_frames ,&dir);
    printf("init  capture got the period frame size: %d \n" , m_frames);
    //设置缓存区大小
    m_periodSize = m_frames*m_sampleSize*m_channels;      //周期缓存区大小 = 帧数×通道数×采样位宽（这里16就是2，所以不用除以8）
    rc = snd_pcm_hw_params_set_buffer_size_near(m_pCaptureHnadel ,m_phw_capture_params , &m_periodSize);
    //    rc = snd_pcm_hw_params_set_buffer_size(m_pCaptureHnadel ,m_phw_params , m_periodSize);
    printf("init  capture period buffer size: %d \n" , m_periodSize);

    if(rc < 0)
    {
        printf("Unable to set buffer size %li : %s\n", m_periodSize, snd_strerror(rc));
    }
    unsigned int val = 0 ;
    snd_pcm_hw_params_get_buffer_size(m_phw_capture_params , (snd_pcm_uframes_t*)&val);
    printf("init  capture got buffer size: %d \n" , val);


    //    snd_pcm_hw_params_get_period_time(m_phw_params , &val , &dir);
    //    printf("init  capture got period time : %d \n" , val);

    //    m_periodSize = (snd_pcm_uframes_t)m_sampleSize*m_channels*m_frames;


    rc = snd_pcm_hw_params(m_pCaptureHnadel, m_phw_capture_params);
    if (rc <0)
    {
        printf("error: %s\n",snd_strerror(rc));
        exit(1);
    }

    //开辟N倍周期数据大小的缓存区
    m_pCaptureBuffer = (char *) malloc(m_periodSize);
    if(m_pCaptureBuffer == NULL)
    {
        printf("Can't Alloc Memery!\n");
    }

    snd_pcm_drop(m_pCaptureHnadel);
    snd_pcm_start(m_pCaptureHnadel);
    snd_pcm_prepare(m_pCaptureHnadel);
    printf("Capture initial successful!\n");
}
void playBackInit()
{
    int ret;
    int dir;

    ret = snd_pcm_open(&m_pPlayBackHandel, "default", SND_PCM_STREAM_PLAYBACK, 0);  //"default"plughw:0,0
    if (ret < 0)
    {
        printf("snd_pcm_open");
        exit(1);
    }

    ret = snd_pcm_hw_params_malloc(&m_phw_playbback_params);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_malloc");
        exit(1);
    }

    ret = snd_pcm_hw_params_any(m_pPlayBackHandel, m_phw_playbback_params);
    if(ret < 0)
    {
        printf("snd_pcm_hw_params_any");
        exit(1);
    }



    ret = snd_pcm_hw_params_set_rate_resample(m_pPlayBackHandel,m_phw_playbback_params,1);
    if(ret < 0)
    {
        printf("snd_pcm_hw_params_set_rate_resample\n");
        exit(1);
    }

    ret = snd_pcm_hw_params_set_access(m_pPlayBackHandel , m_phw_playbback_params , SND_PCM_ACCESS_RW_INTERLEAVED);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_set_access");
        exit(1);
    }

    ret = snd_pcm_hw_params_set_format(m_pPlayBackHandel , m_phw_playbback_params , (snd_pcm_format_t)m_sampleSize);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_set_format");
        exit(1);
    }

    ret = snd_pcm_hw_params_set_rate_near(m_pPlayBackHandel , m_phw_playbback_params , &m_sampleRate, &dir);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_set_rate_near");
        exit(1);
    }

    ret = snd_pcm_hw_params_set_channels(m_pPlayBackHandel , m_phw_playbback_params , m_channels);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_set_channels");
        exit(1);
    }

    //    m_frames = 64 ;
    ret = snd_pcm_hw_params_set_period_size_near(m_pPlayBackHandel , m_phw_playbback_params , &m_frames, &dir);
    printf("init playback period  frame size: %d \n" , m_frames);

    snd_pcm_hw_params_get_period_size(m_phw_playbback_params , &m_frames ,&dir);
    printf("init  playback got the period frame size: %d \n" , m_frames);
    //    m_periodSize = (snd_pcm_uframes_t)m_sampleSize*m_channels*m_frames;

    if (ret < 0)
    {
        printf("Unable to set period size %li : %s\n", m_periodSize,  snd_strerror(ret));
    }

    m_periodSize = m_frames*m_sampleSize*m_channels;      //周期缓存区大小 = 帧数×通道数×采样位宽（这里16就是2，所以不用除以8）
    printf("init playback befor period  buffer size: %d \n" , m_periodSize);

    ret = snd_pcm_hw_params_set_buffer_size_near(m_pPlayBackHandel , m_phw_playbback_params , &m_periodSize);
    //    ret = snd_pcm_hw_params_set_buffer_size(m_pPlayBackHandel ,m_phw_params , m_periodSize);

    printf("init playback period  buffer size: %d \n" , m_periodSize);

    //    m_periodSize = (snd_pcm_uframes_t)m_sampleSize*m_channels*m_frames;

    if(ret < 0)
    {
        printf("Unable to set buffer size %li : %s\n", m_periodSize, snd_strerror(ret));
    }

    ret = snd_pcm_hw_params(m_pPlayBackHandel , m_phw_playbback_params);
    if (ret < 0)
    {
        perror("snd_pcm_hw_params");
        exit(1);
    }

    snd_pcm_drop(m_pPlayBackHandel);
    snd_pcm_start(m_pPlayBackHandel);
    snd_pcm_prepare(m_pPlayBackHandel);
    printf("playback initial successful!\n");
}


AlsaCapthureThread::AlsaCapthureThread()
{
    captureInit();
    setTerminationEnabled(true);
    start();
}
AlsaCapthureThread::~AlsaCapthureThread()
{
//    stopCapture();
}

void AlsaCapthureThread::run()
{

    while (1) {
        //        qDebug()<<"Capture Thread running"<<QThread::currentThreadId()<<isBroadcastThreadRunning;
        if(!isBroadcastThreadRunning)
            break;
        mutex.lock();
        readOnePeriod();
        dataQueue.append(QByteArray(m_pCaptureBuffer , m_periodSize));
        mutex.unlock();
        usleep(10);
    }
    stopCapture();
    exit(0);
}


ALsaPlayBackThread::ALsaPlayBackThread()
{
    playBackInit();
    setTerminationEnabled(true);
    start();
}
ALsaPlayBackThread::~ALsaPlayBackThread()
{
//    stopPlatBack();
}
void ALsaPlayBackThread::run()
{

    while (1) {
        //        qDebug()<<"PlayBack Thread running"<<QThread::currentThreadId();
        if(!isBroadcastThreadRunning && !isReplayRecordFileRunning)
            break;
        mutex.lock();
        while(!dataQueue.isEmpty()){
            playOnePeriod(dataQueue.takeFirst().data() , m_frames);
        }

        if(isReplayRecordFileRunning){
            playRecordSize += m_periodSize;
            if(playRecordSize >= recordFileSize){
                isReplayRecordFileRunning = false ;
                qDebug()<<"Record File Play Over";
            }
        }
        mutex.unlock();
        usleep(10);
    }


    stopPlatBack();
    exit(0);
}


AlsaSoundObject::AlsaSoundObject(QObject *parent) : QObject(parent)
{

    m_pCaptureThread = NULL ;
    m_pPlayBackThread = NULL;
    m_pLoadRcordFileThread = NULL ;
}


void AlsaSoundObject::startAndStopBroadcat(bool isBroadcat , bool isRecord , QString recordPath)
{

    //   qDebug()<<"BBB"<<isBroadcat;

    if(isBroadcat){
//        recordFile.setFileName("./Audio/sysBroadcastRecordFile/18-01-18 18:54:27.pcm");
//        if(!recordFile.open(QFile::ReadOnly)){
//            qDebug()<<"Test Record File Error";
//        }
        if(isRecord){
            recordFile.setFileName("./Audio/"+recordPath+QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")+".pcm");
            if(!recordFile.isOpen()){
                if(!recordFile.open(QFile::Truncate | QFile::WriteOnly)){
                    qDebug()<<"Broadcast record file open error";
                }
                recordDataBuffer.setDevice(&recordFile);
            }
        }

        //        fp = fopen("./Audio/sysBroadcastRecordFile/"+QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")+".pcm" , 'w+');
        //        if(fp == NULL){
        //            qDebug()<<"Broadcast record file open error";
        //            return;
        //        }

        isBroadcastThreadRunning = true ;

        m_pCaptureThread = new AlsaCapthureThread;
        qDebug()<<"Play Thread"<<m_pPlayBackThread;
        if(m_pPlayBackThread == NULL)
            m_pPlayBackThread = new ALsaPlayBackThread;

        if(isReplayRecordFileRunning){
            dataQueue.clear();
            isReplayRecordFileRunning = false;
        }


    }
    else{

        qDebug()<<"Stop B";

        if(m_pCaptureThread == NULL || m_pPlayBackThread == NULL)
            return;

        isBroadcastThreadRunning = false ;
        //        mutex.unlock();


//        m_pPlayBackThread->terminate();
//        m_pPlayBackThread->wait();
//        m_pCaptureThread->terminate();
//        m_pCaptureThread->wait();

        //            delete m_pCaptureThread;
        //            delete m_pPlayBackThread;


        if(recordFile.isOpen()){

            //            qDebug()<<"Write Len:"<<recordFile.write(tmpBuffer)<<"Buffer Len"<<tmpBuffer.length();
            //            recordFile.waitForBytesWritten(2000);
            recordFile.close();
            if(recordFile.size() < 44100*2*2)
                recordFile.remove();

//            recordFile.open(QFile::ReadOnly);
//            QByteArray tmp;
//            tmp = recordFile.readAll();
//            //            qDebug()<<"Read Size:"<<tmpBuffer.readRawData(tmp.data() , recordFile.size());
//            qDebug()<<"File Size:"<<recordFile.size()<<"M_perSize"<<m_periodSize;
//            for(int i = 0 ; i < tmp.length()/m_periodSize ; i++){
//                dataQueue.append(tmp.mid(i*m_periodSize));
//            }
//            qDebug()<<"Record File Replay";

        }

        if(m_pCaptureThread->isRunning()){
            m_pCaptureThread->quit();
            m_pCaptureThread->wait(100);
            m_pCaptureThread->deleteLater();
        }
        if(m_pPlayBackThread->isRunning()){
            m_pPlayBackThread->quit();
            m_pPlayBackThread->wait(100);
            m_pPlayBackThread->deleteLater();
        }


        m_pCaptureThread = NULL;
        m_pPlayBackThread = NULL ;
    }
}

//void threadRecordData(QByteArray &recordBuffer)
//{

//    if(!isReplayRecordFileRunning)
//        return;
//    for(int i = 0 ; i < recordBuffer.length()/m_periodSize ; i++){
//        dataQueue.append(recordBuffer.mid(i*m_periodSize));
//        QThread::msleep(10);
//    }
//}
void LoadFileThread::run()
{
//    if(!isReplayRecordFileRunning){
//        qDebug()<<" *****************Quit Loading Thread";
////        quit();
////        wait(100);
////        deleteLater();
//        return;
//    }


//    mutex.lock();
    qDebug()<<"************************Loading Record Data Start";

    while (1) {
        if(!isReplayRecordFileRunning || sendReciordSize >= recordFileSize)
            break;;
        mutex.lock();
        dataQueue.append(m_RecordData.mid(sendReciordSize , m_periodSize));
        sendReciordSize += m_periodSize;
        if(sendReciordSize >= recordFileSize){
            qDebug()<<"Send Record File Over";
//            isReplayRecordFileRunning = false ;
        }
        mutex.unlock();
    }

//    for(int i = 0 ; i < 2 ; i++){
//        dataQueue.append(m_RecordData.mid(i*m_periodSize));
//        QThread::msleep(5);
//    }



    qDebug()<<"************************Loading Record Data Over";

    exit(0);
}

void AlsaSoundObject::replayBroadcastFile(QString recordFileName , bool isReplay)
{

    if(isReplay){
        QFile recordFile(recordFileName);
        if(!recordFile.open(QFile::ReadOnly)){
            qDebug()<<"Record File Open Erro";
            return;
        }
        isReplayRecordFileRunning = true ;

        QByteArray buffer = recordFile.readAll();
        recordFileSize = buffer.length() ;
        sendReciordSize = 0;
        playRecordSize = 0 ;
//        m_periodSize = m_frames*m_sampleSize*m_channels;      //周期缓存区大小 = 帧数×通道数×采样位宽（这里16就是2，所以不用除以8）
        m_pPlayBackThread = new ALsaPlayBackThread;
        qDebug()<<"Recplay Perzise"<<m_periodSize;

        if(m_pLoadRcordFileThread == NULL){
            m_pLoadRcordFileThread = new LoadFileThread(buffer);
            m_pLoadRcordFileThread->start();
        }


//        QtConcurrent::run(threadRecordData , buffer);
//        for(int i = 0 ; i < buffer.length()/m_periodSize ; i++){
//            dataQueue.append(buffer.mid(i*m_periodSize));
//            QThread::msleep(10);
//        }

    }
    else{
        isReplayRecordFileRunning = false ;
        if(m_pPlayBackThread  == NULL)
            return;

//        m_pPlayBackThread->terminate();
//        m_pPlayBackThread->wait();
//        m_pPlayBackThread->deleteLater();
        sendReciordSize = 0 ;
        dataQueue.clear();

        if(m_pPlayBackThread->isRunning()){
//            m_pPlayBackThread->terminate();
            m_pPlayBackThread->quit();
            m_pPlayBackThread->wait(100);
            m_pPlayBackThread->deleteLater();

        }
        qDebug()<<m_pLoadRcordFileThread;
        if(m_pLoadRcordFileThread->isRunning()){
            qDebug()<<" *****************      Quit Loading Thread";
            m_pLoadRcordFileThread->quit();
            m_pLoadRcordFileThread->wait(100);
            m_pLoadRcordFileThread->deleteLater();
        }


        m_pLoadRcordFileThread = NULL;
        m_pPlayBackThread = NULL ;

        qDebug()<<"CCCCCCCCCCCCCCCCC Close Replay Thread";

//        QThread::msleep(1000);
    }


}
