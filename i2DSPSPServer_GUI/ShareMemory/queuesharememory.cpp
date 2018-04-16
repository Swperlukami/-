#include "queuesharememory.h"


QueueShareMemory::QueueShareMemory(QSharedMemory::AccessMode ModelOne , QSharedMemory::AccessMode ModelTwo , QObject *parent)
    :QObject(parent),SHMOneModel(ModelOne),SHMTwoModel(ModelTwo)
{
    mpSHMOne = new QSharedMemory("SHMOne" , parent);
    if(mpSHMOne->create(1024 , QSharedMemory::ReadWrite)){
        qDebug()<<"Create ShareMemory One OK";
    }
    else if(mpSHMOne->attach(QSharedMemory::ReadWrite)){
        qDebug()<<"Attach ShareMomeory One Ok";
    }
    else{
        qDebug()<<"Attach ShareMomeory One Error"<<mpSHMOne->errorString();;
        return;
    }


    mpSHMTwo = new QSharedMemory("SHMTwo" , parent);
    if(mpSHMTwo->create(1024 , QSharedMemory::ReadWrite)){
        qDebug()<<"Create ShareMemory Two Ok";
    }
    else if(mpSHMTwo->attach(QSharedMemory::ReadWrite)){
        qDebug()<<"Attach ShareMomeory Two Ok";
    }
    else{
        qDebug()<<"ShareMomeory Two Error"<<mpSHMOne->errorString();
        return;
    }

    if(ModelOne == QSharedMemory::ReadWrite){
        mpSHMOne->lock();
        memset(mpSHMOne->data() , 0 , mpSHMOne->size());
        mpSHMOne->unlock();
    }
    else{
        mpSHMTwo->lock();
        memset(mpSHMTwo->data() , 0 , mpSHMTwo->size());
        mpSHMTwo->unlock();
    }

    connect(&mtWriteQueueTimer , &QTimer::timeout , this , &QueueShareMemory::writeSHMDataTimerEvent);
    mtWriteQueueTimer.start(200);
}
QueueShareMemory::~QueueShareMemory()
{
    mpSHMOne->detach();
    mpSHMTwo->detach();
    delete mpSHMOne;
    delete mpSHMTwo;
}

bool QueueShareMemory::getSHMData(QQueue<QByteArray> &QueuData)
{
    if(mReadQueue.isEmpty())
        return false;

    QueuData = mReadQueue;
    mReadQueue.clear();
    return true;
}
void QueueShareMemory::writeSHMData(const QByteArray WriteData)
{
    mWriteQueue.append(WriteData);
}
void QueueShareMemory::writeSHMDataTimerEvent()
{
    if(!mWriteQueue.isEmpty()){
        char *to = NULL;
        if(SHMOneModel == QSharedMemory::ReadWrite){
            if(mpSHMOne->isAttached()){

                mpSHMOne->lock();
                to = (char*)mpSHMOne->data();
                QByteArray Item = mWriteQueue.first();
                if(to[0] == 0){
                    to[0] = 1;
                    memcpy(to+1 , Item.data() , qMin(Item.size() , mpSHMOne->size()));
                    mWriteQueue.removeFirst();
//                    qDebug()<<"SHM One Write Ok Left size:"<<mWriteQueue.size();
                }
                mpSHMOne->unlock();
            }
            else{
                qDebug()<<"SHM One Attach Error"<<mpSHMOne->error();
                return;
            }
        }
        else if (SHMTwoModel == QSharedMemory::ReadWrite){
            if(mpSHMTwo->isAttached()){
//                qDebug()<<"SHM Two Write Ok";

                mpSHMTwo->lock();
                to = (char*)mpSHMTwo->data();
                QByteArray Item = mWriteQueue.first();
                if(to[0] == 0){
                    to[0] = 1;
                    memcpy(to+1 , Item.data()  , qMin(Item.size() , mpSHMTwo->size()));
                    mWriteQueue.removeFirst();
//                    qDebug()<<"SHM Two Write Ok Left size:"<<mWriteQueue.size();
                }
                mpSHMTwo->unlock();
            }
            else{
                qDebug()<<"SHM Two Attach Error"<<mpSHMTwo->errorString();
                return;
            }
        }

    }
    readSHMDataTimerEvent();
}
void QueueShareMemory::readSHMDataTimerEvent()
{

    char *from = NULL;
    if(SHMOneModel == QSharedMemory::ReadOnly){
        if(mpSHMOne->isAttached()){
            mpSHMOne->lock();
            from = (char*)mpSHMOne->data();
            QByteArray Item(mpSHMOne->size() , 0);
            if(from[0] == 1){
                memcpy(Item.data() , from+1 , mpSHMOne->size());
                from[0] = 0;
                memset(mpSHMOne->data() , 0 , mpSHMOne->size());
                mReadQueue.append(Item);
//                qDebug()<<"Server SHM One Read OK:"<<Item.data();
            }
            mpSHMOne->unlock();

        }
        else{
            qDebug()<<"SHM One Attach Error"<<mpSHMOne->errorString();
            return;
        }
    }
    else if (SHMTwoModel == QSharedMemory::ReadOnly){
        if(mpSHMTwo->isAttached()){
            mpSHMTwo->lock();
            from = (char*)mpSHMTwo->data();
            QByteArray Item(mpSHMOne->size() , 0);
            if(from[0] == 1){
                memcpy(Item.data() , from+1 , mpSHMOne->size());
                from[0] = 0;
                memset(from , 0 , mpSHMTwo->size());
                mReadQueue.append(Item);
//                qDebug()<<"Server SHM Two Read OK:"<<Item.data();
            }
            mpSHMTwo->unlock();

        }
        else{
            qDebug()<<"SHM Two Attach Error"<<mpSHMTwo->errorString();
            return;
        }
    }


}

