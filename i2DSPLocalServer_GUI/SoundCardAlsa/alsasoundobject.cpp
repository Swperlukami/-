#include "alsasoundobject.h"
#include <alsa/asoundlib.h>
#include <QQueue>
#include <QDebug>
#include <QMutex>
#include <QFile>
#include <QDateTime>


QFile recordFile;
QMutex mutex;

QQueue<QByteArray>dataQueue;


char *m_pCaptureBuffer;
snd_pcm_uframes_t m_periodSize;
snd_pcm_uframes_t m_frames = 1024;
snd_pcm_t *m_pCaptureHnadel;
snd_pcm_t *m_pPlayBackHandel;
int m_channels = 1 ;
unsigned int m_sampleSize = SND_PCM_FORMAT_S16_LE;
unsigned int m_sampleRate = 44100;
snd_pcm_hw_params_t *m_phw_capture_params;
snd_pcm_hw_params_t *m_phw_playbback_params;


void readOnePeriod()
{
    int ret=0;
    memset(m_pCaptureBuffer , 0 , m_periodSize);
    ret = snd_pcm_readi(m_pCaptureHnadel, m_pCaptureBuffer, m_frames);
    printf("Capture read frame size: %d buffer size:%d \n" , ret , m_periodSize);

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

    printf("playback frame %d \n", m_frames);
//    snd_pcm_hw_params_get_period_size(m_phw_params ,&m_frames , &dir);
    snd_pcm_hw_params_get_buffer_size(m_phw_playbback_params , &val);

//    printf("playback got frame size %d \n", m_frames);
    printf("playback got buffer size %d \n", val);


    if(len != m_frames)
    {
        printf("snd_pcm_writei frames error!\n");
    }

    ret = snd_pcm_writei(m_pPlayBackHandel , buffer , m_frames);  //peroid_frame

    if(recordFile.write(buffer , (int)m_periodSize) != m_periodSize){
        qDebug()<<"write record file error";
    }
    else{
        qDebug()<<"write record file OKOK";

    }


    if (ret == -EPIPE)
    {
        printf("UUUUUUU playback under run \n");

//        snd_pcm_sw_params_set_silence_threshold(m_pPlayBackHandel , m_phw_params , val);
        snd_pcm_prepare(m_pPlayBackHandel);

        usleep(45000);
    }
    else if(ret <0)
    {
        printf("error from writei");
    }
    else if(ret != m_frames){
        printf("short  write : %d" , ret);
    }

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
    start();
}
void AlsaCapthureThread::run()
{

    while (1) {
        qDebug()<<"Capture Thread running"<<QThread::currentThreadId();
        readOnePeriod();
        mutex.lock();

        dataQueue.append(QByteArray(m_pCaptureBuffer , m_periodSize));
        mutex.unlock();
        usleep(2);
    }
}


ALsaPlayBackThread::ALsaPlayBackThread()
{
    playBackInit();
    start();
}
void ALsaPlayBackThread::run()
{
    qDebug()<<"PlayBack Thread running"<<QThread::currentThreadId();

    while (1) {
        mutex.lock();
        if(!dataQueue.isEmpty()){
            playOnePeriod(dataQueue.takeFirst().data() , m_frames);
        }
        mutex.unlock();
        usleep(2);
    }
}
#include <QTimer>

AlsaSoundObject::AlsaSoundObject(QObject *parent) : QObject(parent)
{
}
void AlsaSoundObject::startAndStopBroadcat()
{


   static bool isBroadcat = true ;

   qDebug()<<"BBB"<<isBroadcat;

    if(isBroadcat){
        recordFile.setFileName("./record/"+QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")+".data");

        if(!recordFile.open(QFile::WriteOnly)){
            qDebug()<<"Broadcast record file open error";
        }
        m_pCaptureThread = new AlsaCapthureThread;
        m_pPlayBackThread = new ALsaPlayBackThread;
    }
    else{
        recordFile.close();
        m_pCaptureThread->terminate();
        m_pCaptureThread->wait();
        m_pPlayBackThread->terminate();
        m_pPlayBackThread->wait();

        m_pCaptureThread->deleteLater();
        m_pPlayBackThread->deleteLater();
        m_pCaptureThread == NULL;
        m_pPlayBackThread == NULL ;

    }

    isBroadcat = !isBroadcat;
}
