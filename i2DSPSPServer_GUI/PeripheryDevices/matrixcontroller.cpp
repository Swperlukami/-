#include "matrixcontroller.h"
#include "PeripheryDevices/powermodeserver.h"

extern PowerModeServer *g_pPowerSerialPort;

MatrixController::MatrixController(QObject *parent) : TcpSocketBaseController(parent)
{
    connect(this , &MatrixController::deviceConneted , this , &MatrixController::matrixConneted);
    connect(this , &MatrixController::deviceDisconnted , this , &MatrixController::matrixDisconnted);
    isDiskMountOK = false ;

    m_checkSDDirTimer.setInterval(1*1000);
    connect(&m_checkSDDirTimer , &QTimer::timeout , this , &MatrixController::checkSDDirExist);

//    qDebug()<<FLINE<<QApplication::applicationDirPath();

}
void MatrixController::matrixConneted()
{
    emit matrixEvents("Online" , m_pTcpSocket->peerAddress().toString());
    qDebug()<<FLINE<<"Matrix Online";
}
void MatrixController::matrixDisconnted()
{
    emit matrixEvents("Offline");
    qDebug()<<FLINE<<"Matrix Offline";

}
bool MatrixController::syncMatrixAudioFiles()
{

    QDir sysAudioFileDir(QApplication::applicationDirPath()+"/Audio/sysAudio/");
    QFileInfoList audioFiles = sysAudioFileDir.entryInfoList(QDir::Files);
    foreach (QFileInfo fileInfo, audioFiles) {
        qDebug()<<FLINE<<fileInfo.absoluteFilePath();
        QString newFileName = "/media/i2dsp-chengdu/0403-0201/"+fileInfo.fileName();
        if(QFile::copy(fileInfo.absoluteFilePath() , newFileName)){
            qDebug()<<"Sync File:"<<fileInfo.fileName()<<" Ok";
        }
        else
            qDebug()<<"Sync File:"<<fileInfo.fileName()<<" Error";

    }




    userSetMatrixEvent("Unmount" , 0 , "" , 0);


//    QString sysCopyDirCmd = QString("cp %1/Audio/sysAudio/* /media/i2dsp-chengdu/0403-0201/").arg(QApplication::applicationDirPath());
//    QProcess::execute(sysCopyDirCmd);
//    qDebug()<<FLINE<<"Syn Matrix OK"<<sysCopyDirCmd;

    return true;
}

void MatrixController::processData(QByteArray data)
{
    MARTIX_RESPONSE_HEADER *pRespHeader = (MARTIX_RESPONSE_HEADER*)data.data();
    MARTIX_RESPONSE_1 *pRespInfo = (MARTIX_RESPONSE_1*)(data.data() + sizeof(MARTIX_RESPONSE_HEADER));

//    qDebug()<<FLINE<<"Matrix Feet Back:"<<data;
    if((pRespHeader->szLength+pRespHeader->data_size) < data.length()){
        qDebug()<<FLINE<<"Matrix Packet Len Not Enough Return"<<pRespHeader->szLength+pRespHeader->data_size<<"Rev Data Len :"<<data.length() ;
        return;
    }

    QString respCmd = "" , respState = "";
    switch (pRespHeader->resp_cmd) {
    case MARTIX_CMD_PLAY:
        respCmd = "Play";
        break;
    case MARTIX_CMD_PAUSE:
        respCmd = "Pause";

        break;
    case MARTIX_CMD_CONTINUE:
        respCmd = "Continue";

        break;
    case MARTIX_CMD_SET_VOL:
        respCmd = "Set Vol";

        break;
    case MARTIX_CMD_MOUNT_DISK:
        respCmd = "Mount Disk";

        break;
    case MARTIX_CMD_UNMOUNT_DISK:
        respCmd = "Unmount Disk";

        break;
    }

    switch (pRespInfo->resp_status) {
    case RESP_OK:
        if(pRespHeader->resp_cmd == MARTIX_CMD_MOUNT_DISK){
            m_checkSDDirTimer.start();
        }
        respState = "OK";
        break;
    case RESP_BITRATE_NOTSUPPORT:
        respState = "BitRate Not Support";
        break;
    case RESP_FILE_OPEN_FAILED:
        respState = "File Open Failed";
        break;
    case RESP_FILE_FORMAT_NOTSUPPORT:
        respState = "Format Not Support";

        break;
    }

    qDebug()<<FLINE<<"Matrix Response Cmd:"<<respCmd <<"State:"<<respState;


}
void MatrixController::userSetMatrixEvent(QString cmd, int channelNum, QString songName, int vol)
{
    if(cmd.isEmpty())
        return;

    QByteArray buffer(250,0);
    MARTIX_PACKET_HEADER *pHeader = (MARTIX_PACKET_HEADER*)buffer.data();
    MARTIX_SONG_INFO *pSongInfo = (MARTIX_SONG_INFO*)(buffer.data() + sizeof(MARTIX_PACKET_HEADER));
    MARTIX_VOL *pMatrixChannelVol = (MARTIX_VOL*)(buffer.data() + sizeof(MARTIX_PACKET_HEADER));

    pHeader->szID = MERGEPACKAGE_ID;

    pHeader->channelNum = channelNum;

    float volPercent = ((float)vol)/100.0;

    if(cmd == "Play"){
        qDebug()<<FLINE<<"User Set Channel:"<<channelNum+1<<" Play Song:"<<songName<<"Vol:"<<(unsigned char)MARTIX_MAX_VOL*volPercent;
        pHeader->cmd = MARTIX_CMD_PLAY;
        pHeader->szLength = sizeof(MARTIX_PACKET_HEADER);
        pHeader->data_size = sizeof(MARTIX_SONG_INFO);

        //Open Listen SPK
        g_pPowerSerialPort->m_pListenSPKController->userSetListenSPKEvent("Enable");

        strcpy(pSongInfo->songName , songName.toUtf8().data());
        pSongInfo->cur_vol.vol = (unsigned char)(MARTIX_MAX_VOL*volPercent);
    }
    else if(cmd == "Pause"){
        qDebug()<<FLINE<<"User Pause Channel:"<<channelNum+1;
        pHeader->cmd = MARTIX_CMD_PAUSE;
        pHeader->szLength = sizeof(MARTIX_PACKET_HEADER);

        g_pPowerSerialPort->m_pListenSPKController->userSetListenSPKEvent("Disable");

    }
    else if(cmd == "Continue"){
        qDebug()<<FLINE<<"User Continue Channel:"<<channelNum+1;

        pHeader->cmd = MARTIX_CMD_CONTINUE;
        pHeader->szLength = sizeof(MARTIX_PACKET_HEADER);

    }
    else if(cmd == "Vol"){
        qDebug()<<FLINE<<"User Set Channel:"<<channelNum+1<<"Vol"<<(unsigned char)(MARTIX_MAX_VOL*volPercent);

        pHeader->cmd = MARTIX_CMD_SET_VOL;
        pHeader->szLength = sizeof(MARTIX_PACKET_HEADER);
        pHeader->data_size = sizeof(MARTIX_VOL);

        pMatrixChannelVol->vol = (unsigned char)(MARTIX_MAX_VOL*volPercent);
    }
    else if(cmd == "Mount"){
        qDebug()<<FLINE<<"User Mount Disk:"<<channelNum+1<<"Vol"<<(unsigned char)(MARTIX_MAX_VOL*volPercent);

        pHeader->cmd = MARTIX_CMD_MOUNT_DISK;
        pHeader->szLength = sizeof(MARTIX_PACKET_HEADER);

        isDiskMountOK = false ;
    }
    else if(cmd == "Unmount"){
        qDebug()<<FLINE<<"User UnMount Disk:"<<channelNum+1<<"Vol"<<(unsigned char)(MARTIX_MAX_VOL*volPercent);

        pHeader->cmd = MARTIX_CMD_UNMOUNT_DISK;
        pHeader->szLength = sizeof(MARTIX_PACKET_HEADER);

        isDiskMountOK = false;
    }

    sendTcpSocketData(buffer , pHeader->szLength+pHeader->data_size);
}

bool MatrixController::checkSDDirExist()
{
    QDir sdDir("/media/i2dsp-chengdu/0403-0201/");
    if(sdDir.exists()){
        m_checkSDDirTimer.stop();
        qDebug()<<FLINE<<"Check The SD Exist";
        syncMatrixAudioFiles();
        return true ;
    }
    else
        return false;
}
