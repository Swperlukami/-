#include "rs485.h"
#include <QtCore>
#include <QDebug>

extern unsigned char g_RS485Id;

RS485::RS485(QString serialPortName , QObject * parent) :
    SerialPort(serialPortName , parent)
{
    if(!setSerialPortParity(QSerialPort::MarkParity)){
        qDebug()<<FLINE<<"RS485  Set MarkParity Error";
    }

    m_tClearErrorBufferTimer.setSingleShot(true);
    m_tClearErrorBufferTimer.setInterval(10*1000);
    connect(&m_tClearErrorBufferTimer , &QTimer::timeout , [&](){
        qDebug()<<FLINE<<"USC4000 Send Data Time Out";
        isGotAddrCode = false;
        isGotSynCode = false;
        setSerialPortParity(QSerialPort::MarkParity);
        Buffer.clear();
    } );

//    m_tClearErrorBufferTimer.start();
}
RS485::~RS485()
{

}
void RS485::processSerialData(QByteArray data)
{
//    static QByteArray Buffer(0);
//    static bool isGotAddrCode = false ;
    if(!isGotAddrCode){
        if((data.length() > 0 && data.at(0) > 0 && data.at(0) < 65) || data[0] == 0xff){
            rs485AddrCode = data.at(0);
            if(rs485AddrCode != g_RS485Id)
                return;
            qDebug()<<FLINE<<"Got RS485 USC4100 Addr Code :"<<QString::number(data.at(0));
            isGotAddrCode = true ;
            Buffer.clear();
            m_tClearErrorBufferTimer.start();
            //Set Serial Parity Space
            if(!setSerialPortParity(QSerialPort::SpaceParity))
                qDebug()<<"Serial Can Not Got SynCode Set SpaceParity Error";
            return;
        }
    }

//    static bool isGotSynCode = false ;

    Buffer.append(data);

    foreach (unsigned char tmp, Buffer) {
        qDebug(" buffer : %x",tmp);
    }

    if(!isGotSynCode){
        qDebug()<<"size:"<<Buffer.size()<<"lenght:"<<Buffer.length() ;
//        if(Buffer.length() >= 2){
//            qDebug()<<"Buffer len OK :"<<Buffer.length();
//            qDebug("Syn Data0:%x  Data1:%x" ,(unsigned char)Buffer.at(0) , Buffer.at(1));

//        }
//        else{
//            return;
//        }
//        if((unsigned char)Buffer.at(0) == 0xaa){
//            qDebug()<<"Syn Code 0xaa OK ";
//        }
//        if((unsigned char)Buffer.at(1) == 0x55){
//            qDebug()<<"Syn Code 0x55 OK ";
//        }

        if((Buffer.length() >= 2) && ((unsigned char)Buffer.at(0) == 0xaa) && (Buffer.at(1) == 0x55)){
            qDebug()<<FLINE<<"Got RS485 USC4100 Syn Code : 0xaa 0x55";
            isGotSynCode = true ;
        }
        else{
            qDebug()<<FLINE<<"Not Got SynCode Yet Buffer Size:"<<Buffer.length()<<" Data[0]:"<<Buffer[0];
            if(Buffer.length() >= 2){
                Buffer.clear();
                qDebug()<<FLINE<<"Clear RS485 Buffer";
            }
            return ;
        }
    }


    if(Buffer.length() >= 11){
        USC4000_PACK_HEADER *pUSCHeader = (USC4000_PACK_HEADER*)Buffer.data();
        int dataLen = qToBigEndian(pUSCHeader->dataLen);
        qDebug()<<FLINE<<"Packet Data Len :"<<dataLen;
        qDebug()<<FLINE<<"Cmd Code :"<<pUSCHeader->cmd;

        int checkSum = 0 ;
        switch (pUSCHeader->cmd) {
        case 0x50:{
            if(rs485AddrCode != g_RS485Id)
                return;

            USC4000_PACK_CHECKSUM *tmp = (USC4000_PACK_CHECKSUM*)(Buffer.data() + 7);
            checkSum = tmp->checksum;
            checkSum = qToBigEndian(checkSum);
            if(checkSum != dataLen)
                qDebug()<<FLINE<<"CheckSum Error DataLen:"<<dataLen<<" CheckSum:"<<checkSum;

            qDebug()<<FLINE<<"巡检";
            sendUReplyPolling();
        }
            break;
        case 0x51:{
            if(Buffer.length() < (dataLen + 6 + sizeof(USC4000_PACK_CHECKSUM)))
                return;
            int emgInfoNo = 0 ;
            for( ; emgInfoNo < (dataLen-1)/sizeof(USC4000_FIRE_INFO) ; emgInfoNo++){
                USC4000_FIRE_INFO *pFireInfo  = (USC4000_FIRE_INFO*)(Buffer.data()+ 7 + emgInfoNo*sizeof(USC4000_FIRE_INFO));
                handelUSC4000FireInfo(*pFireInfo);
            }

            USC4000_PACK_CHECKSUM *tmp = (USC4000_PACK_CHECKSUM*)(Buffer.data() + 7 + emgInfoNo*sizeof(USC4000_FIRE_INFO));
            checkSum = tmp->checksum;
            checkSum = qToBigEndian(checkSum);
            if(checkSum != dataLen)
                qDebug()<<FLINE<<"CheckSum Error DataLen:"<<dataLen<<" CheckSum:"<<checkSum;

            qDebug()<<FLINE<<"火警";
        }
            break;
        case 0x52:{
            USC4000_PACK_CHECKSUM *tmp = (USC4000_PACK_CHECKSUM*)(Buffer.data() + 7);
            checkSum = tmp->checksum;
            checkSum = qToBigEndian(checkSum);
            if(checkSum != dataLen)
                qDebug()<<FLINE<<"CheckSum Error DataLen:"<<dataLen<<" CheckSum:"<<checkSum;


            static int resetTimes = 0;
            resetTimes+=1;
            if(resetTimes == 3){
                qDebug()<<FLINE<<"复位:";
                resetTimes = 0;
                emit rs485Event(QString("Reset"));

            }
            qDebug()<<FLINE<<"reset times :"<<resetTimes;
        }
            break;
        case 0x53:{
            USC4000_PACK_CHECKSUM *tmp = (USC4000_PACK_CHECKSUM*)(Buffer.data() + 7);
            checkSum = tmp->checksum;
            checkSum = qToBigEndian(checkSum);
            if(checkSum != dataLen)
                qDebug()<<FLINE<<"CheckSum Error DataLen:"<<dataLen<<" CheckSum:"<<checkSum;

            static int selfTestTimes = 0 ;
            selfTestTimes +=1;
            if(selfTestTimes == 3){
                selfTestTimes = 0;
                qDebug()<<FLINE<<"自检";
                emit rs485Event(QString("SelfTest"));

            }
            qDebug()<<FLINE<<"Self Test Times:"<<selfTestTimes;

        }
            break;
        }
    }
    else{
        qDebug()<<"Data Buffer Len Not Enough :"<<Buffer.length();
        return;
    }

    if(!setSerialPortParity(QSerialPort::MarkParity)){
        qDebug()<<FLINE<<"Handel Rs584 Packet Over Set MarkParity Error";
    }

    isGotAddrCode = false ;
    isGotSynCode = false ;
    Buffer.clear();
    m_tClearErrorBufferTimer.stop();


}
void RS485::sendUReplyPolling()
{
    QByteArray buffer(200 , 0);
    USC4000_PACK_HEADER *pHeader = (USC4000_PACK_HEADER*)(buffer.data()+1);
    USC4000_PACK_CHECKSUM *pCheckSum = (USC4000_PACK_CHECKSUM*)(buffer.data()+ 1 + sizeof(USC4000_PACK_HEADER));

    buffer[0] = rs485AddrCode ;
    pHeader->synCode[0] = 0xaa ;
    pHeader->synCode[1] = 0x55 ;
    pHeader->dataLen = qToBigEndian(0x00000001);
    pHeader->cmd = 0x50 ;
    pCheckSum->checksum = qToBigEndian(0x00000001);

    setSerialPortParity(QSerialPort::SpaceParity);
    wirteSerialPort(buffer , 12);
//    m_pSerialPort->waitForBytesWritten();

}
void RS485::handelUSC4000FireInfo(USC4000_FIRE_INFO fireInfo)
{
    qDebug()<<FLINE<<"USC400 Host No :"<<QString::number(fireInfo.usc4000No);
    qDebug()<<FLINE<<"Loop No:"<<QString::number(fireInfo.loopNo);
    qDebug()<<FLINE<<"Addr No:"<<QString::number(fireInfo.addrNo);
    qDebug()<<FLINE<<"Dev Type:"<<QString::number(fireInfo.devType);
    QString strFireInfo = QString("%1*%2*%3*%4").arg(QString::number(fireInfo.usc4000No))
            .arg(QString::number(fireInfo.loopNo)).arg(QString::number(fireInfo.addrNo))
            .arg(QString::number(fireInfo.devType));

    emit rs485Event(strFireInfo);
}

bool RS485::setSerialPortParity(QSerialPort::Parity parity)
{
    if(!m_pSerialPort->setParity(parity)){
        qDebug()<<FLINE<<"RS485 "<<m_pSerialPort->portName()<<"Set Parity "<<m_pSerialPort->errorString();
        return false;
    }
    else
        return true;
}
