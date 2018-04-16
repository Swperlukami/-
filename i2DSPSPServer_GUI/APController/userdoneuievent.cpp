#include "apcontroller.h"
#include <QApplication>
#include "SysDataBase/database.h"

extern QString g_ListenSPKIP;


bool APController::userDoneQmlUIEvent(QString cmd, QString params)
{

}
void APController::saveInpputPartition(QString apip, int inputno)
{
    QString strQuery = QString("update apinfo set inputno = %1 where apid = '%2' ").arg(inputno).arg(apip);
    if(!exec_database(strQuery)){
        qDebug()<<FLINE<<"update DB InputNo error";
    }else{
        m_hSynAPList.insert(apip , inputno);
        qDebug()<<FLINE<<apip<<" "<<inputno<<"DB Save OK";
    }
}
void APController::synSubAPInputInfo()
{
    QList<QString>APIpList = m_hSynAPList.keys();
    for(int synAPCnt = 0 ; synAPCnt < APIpList.count() ; synAPCnt++){
        emit synAPInputNo(APIpList.at(synAPCnt) , m_hSynAPList.value(APIpList.at(synAPCnt)));
    }
}

void APController::setAPBroadcast(bool isBroadcast , QString apid , int apinputno)
{
    if(isBroadcast){
        emit synAPInputNo(apid , 6 , isBroadcast);
        m_BroadcastAPList.insert(apid , apinputno);
    }
    else{
        if(m_BroadcastAPList.contains(apid))
            m_BroadcastAPList.remove(apid);
        emit synAPInputNo(apid , apinputno , isBroadcast);
    }
}
void APController::setAPPlayBroadcastFile(bool isBFile, QString apid, int apinputno)
{
    if(isBFile){
        emit synAPInputNo(apid , 6);
        m_PlayBFilrAPList.insert(apid , apinputno);
    }
    else{
        if(m_PlayBFilrAPList.contains(apid))
            m_PlayBFilrAPList.remove(apid);
        emit synAPInputNo(apid , apinputno);
    }
}
void APController::saveAPName(QString apip, QString name)
{
    QString strQuery = QString("update apinfo set apname = '%1' where apid = '%2' ").arg(name).arg(apip);
    if(!exec_database(strQuery)){
        qDebug()<<FLINE<<"update DB APName error";
    }else{
        qDebug()<<FLINE<<apip<<" "<<name<<"DB Save OK";
    }
}
void APController::usrSetLightOfAPSPState(bool isDeviceError , bool isVocie)
{
    sysIsDevError = isDeviceError ;
    //    static bool devError = false;
    //    if(!devError && isDeviceError)               //first time error
    //        devError = true ;
    //    else if(devError)                            //already error
    //        return ;
    //    else if(devError && !isDeviceError)          //device be well
    //        devError = false ;
    //    else if(!devError && !isDeviceError)         //devices Ok
    //        return;

    emit setLightOfDevBroEmg(sysIsBro , sysIsEmg ,  sysIsDevError , isVocie);
}
void APController::usrEmgBroadcast(bool isEmg, bool isBoradcast)
{

//    sysSelfTest(4);

//    return ;

    if(sysIsBro && !isBoradcast){     //当关闭一次有效的广播，更新录音文件列表
//        m_pUdpBroadcastClient->startAndStopBroadcast(false);
        m_pBroadcastObject->startAndStopBroadcat(false);

        usrRefreashRecordFile();
        qDebug()<<FLINE<<"Restore  Previous Broadcast AP Input";
        for(int apcnt = 0 ; apcnt < m_BroadcastAPList.count() ; apcnt++){
            emit synAPInputNo(m_BroadcastAPList.keys().at(apcnt) , m_BroadcastAPList.value(m_BroadcastAPList.keys().at(apcnt)));
        }
        m_BroadcastAPList.clear();
        emit synAPInputNo(g_ListenSPKIP , 6);
        qDebug()<<FLINE<<"Braodcst Stop";
    }

    if(!sysIsEmg && isEmg){
        emit setSysIsEmg(isEmg);
    }
    else if(sysIsEmg && !isEmg){
        emit setSysIsEmg(isEmg);
    }

    sysIsBro = isBoradcast ;
    sysIsEmg = isEmg ;

    if(sysIsPalyRecordFile && (sysIsEmg || sysIsBro)){
//        usrPausePlayRecordFile(false , sysIsPalyRecordFile);
        sendRecordFileDataOver();
    }
    else if(sysIsPalyRecordFile && !sysIsEmg && !sysIsBro && !sysIsPlayBroadcatFile)
        usrPausePlayRecordFile(false , sysIsPalyRecordFile);


    if(isBoradcast){
//        m_pUdpBroadcastClient->startAndStopBroadcast(true);
                m_pBroadcastObject->startAndStopBroadcat(true);
        qDebug()<<FLINE<<"Braodcst Start";

    }
    else{
        //        m_pBroadcastObject->startAndStopBroadcat(false);

//        qDebug()<<FLINE<<"Restore  Previous Broadcast AP Input";
//        for(int apcnt = 0 ; apcnt < m_BroadcastAPList.count() ; apcnt++){
//            emit synAPInputNo(m_BroadcastAPList.keys().at(apcnt) , m_BroadcastAPList.value(m_BroadcastAPList.keys().at(apcnt)));
//        }
//        m_BroadcastAPList.clear();
//        m_pUdpBroadcastClient->startAndStopBroadcast(false);
//        qDebug()<<FLINE<<"Braodcst Stop";
    }




    QString dir = QApplication::applicationDirPath();
    if(isEmg){
        m_pEmgSound->play();
        //        QSound::play("/home/i2dsp-chengdu/AdrianProgram/build-EmgBraodcast-Desktop_Qt_5_9_1_GCC_64bit-Debug/i2DSPLocalServer_GUI/Audio/sysBroadcastFile/B1.wav");
    }
    else{
        m_pEmgSound->stop();
    }

//    emit setSysIsEmg(isEmg);


    if(isEmg)
        emit synAPInputNo(g_ListenSPKIP , 6);
    else if(isBoradcast)
        emit synAPInputNo(g_ListenSPKIP , 6);

    emit setLightOfDevBroEmg(sysIsBro , sysIsEmg ,  sysIsDevError , false);


}


bool APController::usrPlayOrStopBroadcastFile(bool isPlay, QString fileName)
{
    //    QSound::play("/home/i2dsp-chengdu/AdrianProgram/build-EmgBraodcast-Desktop_Qt_5_9_1_GCC_64bit-Debug/i2DSPLocalServer_GUI/Audio/sysBroadcastFile/Confirm.wav");

    if(sysIsBro || sysIsEmg)
        return false;

    if(isPlay){
        if(m_pBFilePlayProcess != NULL && m_pBFilePlayProcess->state() == QProcess::Running){
            m_pBFilePlayProcess->kill();
            m_pBFilePlayProcess->deleteLater();
            m_pBFilePlayProcess = NULL;
        }

//        m_pUdpBroadcastClient->startAndStopBroadcast(true);
        emit synAPInputNo(g_ListenSPKIP , 6);
        if(sysIsPalyRecordFile)
            usrPausePlayRecordFile(true , sysIsPalyRecordFile);

        sysIsPlayBroadcatFile = true ;
        QString dir = QApplication::applicationDirPath();
        //        qDebug()<<FLINE<<isPlay<<dir<<fileName;
        m_pBFilePlayProcess = new QProcess(this);
        QString cmd = QString("mplayer -slave -loop 0 -volume 40 \"%1\"").arg(dir+"/Audio/sysBroadcastFile/"+fileName);
        if(m_pBFilePlayProcess != NULL){
            m_pBFilePlayProcess->start(cmd);
            qDebug()<<FLINE<<"Play Broadcst File";

            return true ;
        }

    }
    else{
//        if(sysIsPalyRecordFile)
//            usrPausePlayRecordFile(false , sysIsPalyRecordFile);

//        m_pUdpBroadcastClient->startAndStopBroadcast(false);
        sysIsPlayBroadcatFile = false ;
        qDebug()<<FLINE<<"Restore  Previous BFile AP Input";
        for(int apcnt = 0 ; apcnt < m_PlayBFilrAPList.count() ; apcnt++){
            emit synAPInputNo(m_PlayBFilrAPList.keys().at(apcnt) , m_PlayBFilrAPList.value(m_PlayBFilrAPList.keys().at(apcnt)));
            emit setAPPlayBFile(m_PlayBFilrAPList.keys().at(apcnt) , false);
        }
        m_PlayBFilrAPList.clear();

        if(m_pBFilePlayProcess != NULL && m_pBFilePlayProcess->state() == QProcess::Running){
            m_pBFilePlayProcess->kill();
            m_pBFilePlayProcess->deleteLater();
            m_pBFilePlayProcess = NULL;
            qDebug()<<FLINE<<"Stop Broadcst File";
            return false ;
        }
    }



    //    QSound::play(dir+"/Audio/sysBroadcastFile/"+fileName);


    //    QSoundEffect effect;
    //    effect.setSource(QUrl::fromLocalFile(dir+"/Audio/sysBroadcastFile/"+fileName));
    //    effect.setLoopCount(QSoundEffect::Infinite);
    //    effect.setVolume(1.0);
    //    effect.play();

    //    QMediaService mediaService(this);
    //    QMediaObject mediaObject(this , &mediaService);
    //    QMediaPlayer *player = new QMediaPlayer(this , QMediaPlayer::StreamPlayback);
    //    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    //    player->setMedia(QUrl::fromLocalFile(dir+"/Audio/sysBroadcastFile/"+fileName));
    //    player->setVolume(100);
    //    player->play();


    //    if(isPlay){
    //        m_pPlayBroadcastFile = new QSound(dir+"/Audio/sysBeaodcastFile/"+fileName);
    //        m_pPlayBroadcastFile->setLoops(QSound::Infinite);
    //        qDebug()<<m_pPlayBroadcastFile->fileName();
    //        m_pPlayBroadcastFile->play();
    //    }
    //    else{
    //        m_pPlayBroadcastFile->stop();
    //        delete m_pPlayBroadcastFile;
    //    }
}


void APController::usrPausePlayBroadcastFile(bool isPause , bool isPlayingBFile)
{
    if(!isPlayingBFile)
        return;

    static bool pause = false ;
    if(isPlayingBFile && pause && isPause)
        return;

    if(isPlayingBFile && !isPause && pause){
        pause = false ;
        emit synAPInputNo(g_ListenSPKIP , 6);
        if(m_pBFilePlayProcess != NULL && m_pBFilePlayProcess->state() == QProcess::Running){
            m_pBFilePlayProcess->write(QByteArray("pause\n").data());
        }
        continuPlayBFile();
        qDebug()<<FLINE<<"Continu Play";
    }

    if(isPlayingBFile && isPause && !pause){
        pause = true ;
        if(m_pBFilePlayProcess != NULL && m_pBFilePlayProcess->state() == QProcess::Running){
            m_pBFilePlayProcess->write(QByteArray("pause\n").data());
        }
        qDebug()<<"Pause Play";
    }

}
void APController::continuPlayBFile()
{
    emit setAPPlayBFile("" , true);

    for(int apcnt = 0 ; apcnt < m_PlayBFilrAPList.count() ; apcnt++){
        emit synAPInputNo(m_PlayBFilrAPList.keys().at(apcnt) , 6);
        emit setAPPlayBFile(m_PlayBFilrAPList.keys().at(apcnt) , true);
    }
}
bool APController::usrPlayRecordFile(bool isPlay , QString recordFileName)
{


    if(isPlay){
        if(sysIsEmg || sysIsBro || sysIsPlayBroadcatFile)
            return false ;

        emit synAPInputNo(g_ListenSPKIP , 6);

//        m_pAudioPCMOutput->playOrStopPCMAudioFile(true , "./Audio/sysBroadcastRecordFile/"+recordFileName);
        m_pBroadcastObject->replayBroadcastFile("./Audio/sysBroadcastRecordFile/"+recordFileName , true);
        sysIsPalyRecordFile = true ;
        qDebug()<<FLINE<<"Playing Record File "<<recordFileName;
        return true ;
    }
    else{
        qDebug()<<FLINE<<"Stop Record File "<<recordFileName;
        m_pBroadcastObject->replayBroadcastFile("./Audio/sysBroadcastRecordFile/"+recordFileName , false);

//        m_pAudioPCMOutput->playOrStopPCMAudioFile(false , "");

        sysIsPalyRecordFile = false ;
        return false;
    }
}
void APController::usrPausePlayRecordFile(bool isPause, bool isPlayingRecord)
{
    if(isPause && isPlayingRecord){
        qDebug()<<FLINE<<"Pause Play Record File ";
//        m_pAudioPCMOutput->isPausePCMAudioFile(true);
        m_pBroadcastObject->replayBroadcastFile("", false);
        sendRecordFileDataOver();
    }

    if(!isPause && isPlayingRecord){
        qDebug()<<FLINE<<"Continu Play Record File ";
        emit synAPInputNo(g_ListenSPKIP , 6);
//        m_pAudioPCMOutput->isPausePCMAudioFile(false);

    }
}
void APController::sendRecordFileDataOver()
{
    qDebug()<<FLINE<<"Recod File Play Over";
    emit recordFilePlayOver();
    sysIsPalyRecordFile = false ;

}


