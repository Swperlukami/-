#include "tcpsocket.h"
#include "ap_protocol.h"
#include <QTime>
#include <QThread>

void TcpSocket::processData(const QByteArray cmd)
{
    NETPACKET_HEADER *pHeader = (NETPACKET_HEADER*)cmd.data();
    NETPACKET_AP_STATE *pAPState = (NETPACKET_AP_STATE*)(cmd.data() + sizeof(NETPACKET_HEADER));
    NETPACKET_SP_STATE *pSPState = (NETPACKET_SP_STATE*)(cmd.data() + sizeof(NETPACKET_HEADER));

//    qDebug()<<FLINE<<"Tcp Buf:"<<cmd.size()<<cmd;
    if(pHeader->PacketSize > cmd.length()){
        qDebug()<<FLINE<<"Tcp Buffer Size Error"<<pHeader->PacketSize<<cmd.length();
        return;
    }

    qDebug()<<FLINE<<"Time:"<<QTime::currentTime().toString("mm:ss");

    if(pHeader->Flag == CMD_AP_KEEP_LIVE){
        if(mtCheckLTOnlineTimer.isActive()){
            m_intClinetLive = 3;
        }
        qDebug()<<FLINE<<this->peerAddress()<<"socket:"<<this->socketDescriptor()<<"Keep Live"<<m_intClinetLive;

    }
    else if(pHeader->Flag == CMD_AP_STATE){
        m_intClinetLive = 3;
        APState apstate = APOffline ;
        switch (pAPState->APState) {
        case 1:                  //正常
            qDebug()<<FLINE<<this->peerAddress()<<"AP OK";
            apstate = APOK ;
            break;
        case 2:                  //短路
            qDebug()<<FLINE<<this->peerAddress()<<"AP Short";
            apstate = APShort ;
            break;
        case 3:                  //断路
            qDebug()<<FLINE<<this->peerAddress()<<"AP Open";
            apstate = APOPen ;
            break;
        case 4:                  //过载
            qDebug()<<FLINE<<this->peerAddress()<<"AP Overdrive";
            apstate = APVoerDrive ;
            break;
        case 5:                  //保护
            qDebug()<<FLINE<<this->peerAddress()<<"AP Protect";
            apstate = APProtect ;
            break;
        default:
            break;
        }
        sendUIAPState(apstate);

    }
    else if(pHeader->Flag == CMD_SP_STATE){
        m_intClinetLive = 3;
        for(int spindex = 0 ; spindex < MAX_SP_CNT ; spindex++){
            int SPSate = 0;
            if(pSPState->SPState[spindex] == 0){
                SPSate = 0X63;
            }
            else{
                SPSate = 0x10;
            }
            qDebug()<<FLINE<<"SP "<<spindex+1<<"State:"<<SPSate;

        }

    }

    if(cmd.length() > pHeader->PacketSize){
        qDebug()<<FLINE<<"Handel LianBao ";
        processData(cmd.mid(pHeader->PacketSize));
    }
}

void TcpSocket::sendUIAPState(const APState spstate)
{
    QByteArray buffer(50, 0);
    UI_PACKET_HEAD *pUIPackHead = (UI_PACKET_HEAD*)buffer.data();
    UI_AP_STATE *pUIPackAPState = (UI_AP_STATE*)(buffer.data() + sizeof(UI_PACKET_HEAD));

    pUIPackHead->Flag = CMD_AP_STATE ;
    pUIPackHead->PacketSize = sizeof(UI_PACKET_HEAD) + sizeof(UI_AP_STATE);
    memcpy(pUIPackHead->APIp , m_strClientIP.toLatin1().data(), IP_LEN);

    pUIPackAPState->spstate = spstate ;

    emit apEvent(buffer);

}


//设置功放事件
void TcpSocket::setAPEmgMode(bool isEmgMode)
{
    QByteArray buffer(10 , 0);
    NETPACKET_HEADER *pHeader = (NETPACKET_HEADER*)buffer.data();
    pHeader->PacketSize = sizeof(NETPACKET_HEADER);

    if(isEmgMode)
        pHeader->Flag = CMD_MC_EMG ;
    else
        pHeader->Flag = CMD_MC_CANCEL_EMG ;
   if(!this->writeSocketData(buffer , pHeader->PacketSize))
       qDebug()<<FLINE<<"Set AP :"<<m_strClientIP<<"Mode :"<<isEmgMode<<" EmgMode Erro";
   else
       qDebug()<<FLINE<<"Set AP :"<<m_strClientIP<<"Mode :"<<isEmgMode<<" EmgMode OKKK";

}
void TcpSocket::setAPInput(int InputNo )
{
    QByteArray buffer(1024 , 0);
    NETPACKET_HEADER *pHeader = (NETPACKET_HEADER*)buffer.data();
    NETPACKET_AP_INPUT *pAPInput = (NETPACKET_AP_INPUT*)(buffer.data() + sizeof(NETPACKET_HEADER));

    pHeader->Flag =  CMD_AP_INPUT;
    pHeader->PacketSize = sizeof(NETPACKET_HEADER)+sizeof(NETPACKET_AP_INPUT);

    pAPInput->InputNo = (unsigned char)InputNo;

    if(!this->writeSocketData(buffer , pHeader->PacketSize)){
        qDebug()<<FLINE<<"Set Ap "<<m_strClientIP<<"input "<<InputNo<<" Error";
    }
    else
        qDebug()<<FLINE<<"Syn Input Ip:"<<m_strClientIP<<" No"<<InputNo ;
}
void TcpSocket::setAPVoice(unsigned char voice)
{
    QByteArray buffer(10 , 0);
    NETPACKET_HEADER *pHeader = (NETPACKET_HEADER*)buffer.data();
    NETPACKET_AP_VOL *pAPVoice = (NETPACKET_AP_VOL*)(buffer.data() + sizeof(NETPACKET_HEADER));

    pHeader->Flag = CMD_AP_VOL;
    pHeader->PacketSize = sizeof(NETPACKET_HEADER);
    pAPVoice->APVol = voice;

    if(!this->writeSocketData(buffer , pHeader->PacketSize))
        qDebug()<<FLINE<<"Set Ap "<<m_strClientIP<<"Vocie "<<voice<<" Error";
    else
        qDebug()<<FLINE<<"Set Ap "<<m_strClientIP<<"Vocie "<<voice<<" OKKK";

}
void TcpSocket::setAPSelfTest()
{
    qDebug()<<FLINE<<"AP Self Test";
    QByteArray buffer(10 , 0);
    NETPACKET_HEADER *pPacket = (NETPACKET_HEADER*)buffer.data();
    pPacket->Flag = CMD_SELF_TEST;
    pPacket->PacketSize = sizeof(NETPACKET_HEADER);
    this->writeSocketData(buffer , pPacket->PacketSize);
}

void TcpSocket::sendAPKeepLiveEvent()
{
    QByteArray buffer(10 , 0);
    NETPACKET_HEADER *pHeader = (NETPACKET_HEADER*)buffer.data();
    pHeader->Flag = CMD_AP_KEEP_LIVE;
    pHeader->PacketSize = sizeof(NETPACKET_HEADER);


    qDebug()<<FLINE<<"Send AP"<<m_strClientIP<<"socket :"<<mSocketDescriptor<<" Kepp Live";
    writeSocketData(buffer , pHeader->PacketSize);

    QThread::msleep(10);
    qDebug()<<FLINE<<"Timer Syn AP Input :"<<this->peerAddress()<<" socket:"<<mSocketDescriptor;
    setAPInput(m_CurrentInputNo);
}
