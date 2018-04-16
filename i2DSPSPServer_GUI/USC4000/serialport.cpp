#include "serialport.h"
#include "PeripheryDevices/periphery_protocol.h"


SerialPort::SerialPort(QString serialPortName, QObject *parent) :
    m_strSerialPortName(serialPortName) , QObject(parent)
{

//    qDebug()<<"Make Serial Port Giveb Name:"<<serialPortName<<" Menber Name"<<m_strSerialPortName;
    m_pSerialPort = new QSerialPort(this);

    connect(m_pSerialPort , &QSerialPort::readyRead , this , &SerialPort::readSerialData);

    connect(&m_tSerialPortCheckTimerl , &QTimer::timeout , this , &SerialPort::checkSerialPortOK);
    connect(&m_tSerialPortCheckTimerl , &QTimer::timeout , this , &SerialPort::openRightSerialPort);

    m_isOpenSerialPort = false;

}
SerialPort::~SerialPort()
{
    delete m_pSerialPort;
}
void SerialPort::setSerialPortFormat(QSerialPort::BaudRate baudRate, QSerialPort::DataBits data, QSerialPort::Parity parity, QSerialPort::StopBits stopBit)
{
    m_pSerialPort->setBaudRate(baudRate);
    m_pSerialPort->setDataBits(data);
    m_pSerialPort->setParity(parity);
    m_pSerialPort->setStopBits(stopBit);
}
void SerialPort::setSerialPortName(QString portName)
{
    m_strSerialPortName = portName ;
    m_tSerialPortCheckTimerl.start(2000);
    qDebug()<<FLINE<<"Set Port Name:"<<m_strSerialPortName ;
}
void SerialPort::wirteSerialPort(QByteArray data, int len)
{
    if(m_pSerialPort->isOpen()){
        if(len != m_pSerialPort->write(data.data() , len))
            qDebug()<<"Write Serial Port Data Error";
    }
}
void SerialPort::checkSerialPortOK()
{
    if(!m_isOpenSerialPort)
        return;

    foreach (QSerialPortInfo SerialInfo , QSerialPortInfo::availablePorts()) {
        if(SerialInfo.portName() == m_pSerialPort->portName())
            return;
    }
    if(m_pSerialPort->isOpen()){
        m_pSerialPort->close();
    }
    m_isOpenSerialPort = false;
    qDebug()<<FLINE<<"SerialPort"<<m_strSerialPortName<<" Offline";

}

void SerialPort::openRightSerialPort()
{
//    qDebug()<<"Serial Timer:"<<m_strSerialPortName;
    if(m_isOpenSerialPort)
        return;

    bool isFineTheSerial = false;
    foreach (QSerialPortInfo SerialInfo , QSerialPortInfo::availablePorts()) {
//        qDebug()<<SerialInfo.portName()<<" My Port Name:"<<m_strSerialPortName;
//        qDebug()<<"Serial Name:"<<SerialInfo.portName()<<" Describle:"<<SerialInfo.description()<<"manu :"<<SerialInfo.manufacturer()<<" Serial No:"<<SerialInfo.serialNumber();

        if(SerialInfo.portName() == m_strSerialPortName){
            qDebug()<<FLINE<<"Serial Port"<<m_strSerialPortName<<" Online";
            isFineTheSerial = true ;
            break;
        }
    }

    if(!isFineTheSerial){
        qDebug()<<FLINE<<"Sys Can Not Find Serial Port :"<<m_strSerialPortName;
        return;
    }

    m_pSerialPort->setPortName(m_strSerialPortName);

    if(m_pSerialPort->open(QIODevice::ReadWrite)){
        qDebug()<<FLINE<<"Serial "<<m_strSerialPortName<<" Open OK";
        m_isOpenSerialPort = true ;
    }
    else{
        qDebug()<<FLINE<<"Serial "<<m_strSerialPortName<<" Open Failed";
    }

}

void SerialPort::readSerialData()
{
    while (m_pSerialPort->bytesAvailable() > 0) {
        processSerialData(m_pSerialPort->readAll());
    }
}
