#include "audiofilesmanagecontroller.h"
#include <QDir>
#include "uicontroller.h"
#include "PeripheryDevices/powermodeserver.h"

extern PowerModeServer *g_pPowerSerialPort;
extern UIController *g_pUIController;


AudioFilesManageController::AudioFilesManageController(QObject *parent) : QObject(parent)
{
    m_pMplayerGetMp3InfoProcess = new QProcess(this);
    //    QString getMp3InfoCmdStr = QString("mplayer -slave -quiet -idle");
    //    m_pMplayerGetMp3InfoProcess->start("");
    //    m_pMplayerGetMp3InfoProcess->waitForFinished(2000);
    //    qDebug()<<FLINE<<m_pMplayerGetMp3InfoProcess->readAll();

    //    addAudioFileToSysFloder( 2 ,"/home/i2dsp-chengdu/音乐/假如爱有天意.mp3");
    //    deleteTheAudioFile("假如爱有天意.mp3");

    //    getTheMp3ID3Info("/home/i2dsp-chengdu/音乐/假如爱有天意.mp3");
//        mediainfoGetAudioInfo("/home/i2dsp-chengdu/音乐/DV.mp3");
}
AudioFilesManageController::~AudioFilesManageController()
{
    m_pMplayerGetMp3InfoProcess->terminate();
    delete m_pMplayerGetMp3InfoProcess;
}

void AudioFilesManageController::initSysEmgRecordFiles()
{
    QDir recordFileDir("./Audio/sysEmgTalkRecordFile/");
    QFileInfoList recordFileList;

    recordFileList = recordFileDir.entryInfoList(QStringList()<<"*" , QDir::Files | QDir::NoDotAndDotDot , QDir::Time);

    for(int recordcnt = 0 ; recordcnt < (recordFileList.count() > 100 ? 100 : recordFileList.count()) ; recordcnt++){
        QFileInfo file = recordFileList.at(recordcnt);
        qDebug()<<FLINE<<"Init Emg Record File:"<<file.fileName();
        QString size = "";
        if(file.size()/1024 < 1)
            size = QString::number(file.size())+"bytes";
        else if(file.size()/1024 >= 1 && file.size()/1024 < 1024)
            size = QString::number(file.size()/1024)+"K";
        else
            size = QString::number(file.size()/(1024*1024))+"M";

        emit initSyaAudioFilesInfo(1 , file.fileName() , "" , "" , 0);

    }

}
void AudioFilesManageController::initSysTalkRecordFiles()
{
    QDir recordFileDir("./Audio/sysTalkRecordFile/");
    QFileInfoList recordFileList;

    recordFileList = recordFileDir.entryInfoList(QStringList()<<"*" , QDir::Files | QDir::NoDotAndDotDot , QDir::Time);

    for(int recordcnt = 0 ; recordcnt < (recordFileList.count() > 100 ? 100 : recordFileList.count()) ; recordcnt++){
        QFileInfo file = recordFileList.at(recordcnt);
        qDebug()<<FLINE<<file.fileName();
        QString size = "";
        if(file.size()/1024 < 1)
            size = QString::number(file.size())+"bytes";
        else if(file.size()/1024 >= 1 && file.size()/1024 < 1024)
            size = QString::number(file.size()/1024)+"K";
        else
            size = QString::number(file.size()/(1024*1024))+"M";

        emit initSyaAudioFilesInfo(3 , file.fileName() , "" , "" , 0);
    }
}
void AudioFilesManageController::initSystemAudoFiles()
{
    QString queryStr = QString("select * from audiofilesinfo");
    QSqlQuery query;
    if(query_broadcast_database(queryStr , query)){
        while (query.next()) {
            emit initSyaAudioFilesInfo(query.value("type").toUInt() , query.value("fileName").toString() , query.value("singer").toString() , query.value("album").toString() , query.value("duration").toInt());
        }
    }
    initSysEmgRecordFiles();
    initSysTalkRecordFiles();
}
bool AudioFilesManageController::addAudioFileToSysFloder(int fileType, QString filePath)
{
    QFile audioFile(filePath);
    if(!audioFile.copy(QString("./Audio/sysAudio/%1").arg(filePath.mid(filePath.lastIndexOf('/')+1)))){
        qDebug()<<FLINE<<"Copy File Error"<<audioFile.errorString();
        return false;
    }



    QStringList fileInfo = mediainfoGetAudioInfo(filePath).split('|');
    qDebug()<<FLINE<<fileInfo;
    QString strExec = QString("insert into audiofilesinfo values('%1' , '%2' , '%3' , %4 , %5);")
            .arg(fileInfo.at(0))
            .arg(fileInfo.at(1))
            .arg(fileInfo.at(2))
            .arg(fileInfo.at(3).toInt())
            .arg(fileType);

    if(!exec_broadcast_database(strExec))
        return false;
    else{
        emit initSyaAudioFilesInfo(fileType , fileInfo.at(0) ,fileInfo.at(1) ,fileInfo.at(2) ,fileInfo.at(3).toInt() );
        return true ;
    }

}
void AudioFilesManageController::deleteTheAudioFile(QString fileName, QString fileType)
{
    //0 EmgRecord 1 Emg 2 TalkRecord 3 Music 4 Bells

    if(fileType != "0" && fileType != "2"){
        QFile audioFile("./Audio/sysAudio/"+fileName);
        if(audioFile.exists()){
            if(audioFile.remove()){
                QString execStr = QString("delete from audiofilesinfo where filename = '%1' ").arg(fileName);
                if(!exec_broadcast_database(execStr)){

                }
                else
                    qDebug()<<FLINE<<"User Delete File"<<fileName;
            }
        }
    }
    else if(fileType == "0"){
        QFile audioFile("./Audio/sysEmgTalkRecordFile/"+fileName);
        if(audioFile.remove())
            qDebug()<<FLINE<<"Remove Emg Record File :"<<fileName;

    }
    else if(fileType == "2"){
        QFile audioFile("./Audio/sysTalkRecordFile/"+fileName);
        if(audioFile.remove())
            qDebug()<<FLINE<<"Remove Talk Record File :"<<fileName;
    }
}
void AudioFilesManageController::userSetAudioManageEvents(QString cmdType, QVariant params, QVariant params2)
{
    if(cmdType == "ReplayAudiodFile"){
        if(params2.toInt() != 0 && params2.toInt() != 2){  //General File

        }
        else if(params2.toInt() == 0){    //Emg Record File
            g_pPowerSerialPort->m_pListenSPKController->userSetListenSPKEvent("Enable");
            g_pUIController->m_EventControllers.pSysBroadcastController->userSetPCEvents("ReplayPCMFile" , "./Audio/sysEmgTalkRecordFile/"+params.toString());
        }
        else if(params2.toInt()  == 2){   //Talk Record File

        }

    }
    else if(cmdType == "StopReplayAudiodFile"){
        g_pUIController->m_EventControllers.pSysBroadcastController->userSetPCEvents("StopReplayPCMFile");
        g_pPowerSerialPort->m_pListenSPKController->userSetListenSPKEvent("Disable");

    }
}

QString AudioFilesManageController::getTheMp3ID3Info(QString mp3FilePath)
{
    //    QString getMp3InfoCmdStr1 = QString("mplayer -slave -quiet -loop 1 -volume 50 \"%1\" ").arg(mp3FilePath);
    //    //    QString getMp3InfoCmdStr1 = QString("mplayer -slave -quiet -idle");

    //    m_pMplayerGetMp3InfoProcess->start(getMp3InfoCmdStr1.toLatin1().data());
    //    m_pMplayerGetMp3InfoProcess->waitForFinished(2000);
    //    qDebug()<<FLINE<<m_pMplayerGetMp3InfoProcess->readAll();


    //    qDebug()<<FLINE<<"Got Mp3 File :"<<mp3FilePath;
    //    //    QString getMp3InfoCmdStr = QString("loadfile \'%1\'\n").arg(mp3FilePath);
    //    //    qDebug()<<FLINE<<"Mplayer Cmd :"<<getMp3InfoCmdStr;
    //    //    m_pMplayerGetMp3InfoProcess->write(getMp3InfoCmdStr.toLatin1().data());
    //    //    m_pMplayerGetMp3InfoProcess->waitForFinished(4000);
    //    //    qDebug()<<FLINE<<"LoadFile:"<<m_pMplayerGetMp3InfoProcess->readAll();

    //    m_pMplayerGetMp3InfoProcess->write(QByteArray("get_time_length\n").data());
    //    m_pMplayerGetMp3InfoProcess->waitForFinished(2000);
    //    //    m_pMplayerGetMp3InfoProcess->write(QByteArray("get_meta_album \n").data());
    //    //    m_pMplayerGetMp3InfoProcess->waitForFinished(100);


    //    //    qDebug()<<FLINE<<"Get Mp3 Time:"<<m_pMplayerGetMp3InfoProcess->readAll();
    //    QString outputStr = QString(m_pMplayerGetMp3InfoProcess->readAll());

    //    m_pMplayerGetMp3InfoProcess->kill();


    //    qDebug()<<FLINE<<"Get Time Length:"<<outputStr;
    //    qDebug()<<FLINE<<outputStr.mid(outputStr.lastIndexOf("ANS_LENGTH=")+11).simplified();
    //    int mp3Duration = outputStr.mid(outputStr.lastIndexOf("ANS_LENGTH=")+11).simplified().toDouble();

    //    qDebug()<<FLINE<<"Got Mp3 Duration:"<<mp3Duration;

    //    QFile mp3File(mp3FilePath);
    //    if(!mp3File.exists()){
    //        qDebug()<<FLINE<<"User Selected AUdio File:"<<mp3FilePath<<" Not Exist";
    //        return "" ;
    //    }

    //    if(!mp3File.open(QFile::ReadOnly)){
    //        qDebug()<<FLINE<<"Audio File "<<mp3FilePath<<" OPen Errer";
    //        return "";
    //    }

    //    if(!mp3File.seek(mp3File.size()-128)){
    //        qDebug()<<FLINE<<"Seek File Error";
    //    }

    //    qDebug()<<FLINE<<mp3FilePath;
    //    qDebug()<<FLINE<<"TAG Header:"<<QString(mp3File.read(3));
    //    //    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    //    qDebug()<<FLINE<<"TAG Title:"<<QString(mp3File.read(30)) ;
    //    //    qDebug()<<FLINE<<"TAG Artist:"<<QString(mp3File.read(30));
    //    //    qDebug()<<FLINE<<"TAG Album:"<<QString(mp3File.read(30));
    //    //    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UFT-8"));

    //    //    qDebug()<<FLINE<<"ID3 Header:"<<QString(mp3File.read(3));


    //    QString audioFileName = mp3FilePath.mid(mp3FilePath.lastIndexOf('/')+1);
    //    QString audioSinger ="N/A" ;     //QString(mp3File.read(30));
    //    QString audioAlnum ="N/A";       //QString(mp3File.read(30));
    //    QString audioDuration = QString::number(mp3Duration);
    //    mp3File.close();

    //    QString fileInfo = QString("%1|%2|%3|%4").arg(audioFileName).arg(audioSinger).arg(audioAlnum).arg(audioDuration);

    //    qDebug()<<FLINE<<"Got mp3 File Info:"<<fileInfo;
    //    return fileInfo;
}
QString AudioFilesManageController::mediainfoGetAudioInfo(QString mp3FilePath)
{
    QString getMp3InfoCmdStr1 = QString("mediainfo %1 ").arg(mp3FilePath);

    //    qDebug()<<FLINE<<getMp3InfoCmdStr1;
    m_pMplayerGetMp3InfoProcess->start(getMp3InfoCmdStr1);
    m_pMplayerGetMp3InfoProcess->waitForFinished(5000);
    m_pMplayerGetMp3InfoProcess->kill();

    QString mediainfo = QString(m_pMplayerGetMp3InfoProcess->readAll());
    //    qDebug()<<FLINE<<"MediaInfo Got Info:"<<mediainfo.split('\n');

    QString audioFileName = mp3FilePath.mid(mp3FilePath.lastIndexOf('/')+1);
    QString audioSinger ;
    QString audioAlnum ;
    int audioDuration ;

    foreach (QString info, mediainfo.split('\n')) {
        if(info.contains("File size")){
            qDebug()<<FLINE<<info;
            info =  info.mid(info.lastIndexOf(": ")+2);
            info = info.mid(0,info.lastIndexOf(' '));
            qDebug()<<FLINE<<info.toFloat();

        }
        else if(info.contains("Duration")){
            qDebug()<<FLINE<<info;
            info = info.mid(info.lastIndexOf(": ")+2);
            QString min , second , mssecond;
            if(info.contains("mn")){
                min = info.split(' ').at(0);
                second = info.split(' ').at(1);
                min = min.mid(0 , min.lastIndexOf("mn"));
                second = second.mid(0 , second.lastIndexOf("s"));

                audioDuration = min.toInt()*60 + second.toInt() ;
            }
            else if(info.contains("ms")){
                second = info.split(' ').at(0);
                mssecond = info.split(' ').at(1);
                second = second.mid(0 , second.lastIndexOf("s"));
                mssecond = mssecond.mid(0 , mssecond.lastIndexOf("ms"));

                audioDuration = second.toInt();
            }

            qDebug()<<FLINE<<min;
            qDebug()<<FLINE<<second;
            qDebug()<<FLINE<<mssecond;


        }
        else if(info.contains("Album")){
            qDebug()<<FLINE<<info;
            info = info.mid(info.lastIndexOf(": ")+2);
            qDebug()<<FLINE<<info;
            audioAlnum = info ;
        }
        else if(info.contains("Performer")){
            qDebug()<<FLINE<<info;
            info = info.mid(info.lastIndexOf(": ")+2);
            qDebug()<<FLINE<<info.toUtf8();

            audioSinger = info ;
        }
        else if(info == "" ){
            break;
        }
    }

    QString fileInfo = QString("%1|%2|%3|%4").arg(audioFileName).arg(audioSinger).arg(audioAlnum).arg(audioDuration);
    qDebug()<<FLINE<<fileInfo;

    return fileInfo;



}
