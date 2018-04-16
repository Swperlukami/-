#ifndef QUEUESHAREMEMORY_H
#define QUEUESHAREMEMORY_H

#include <QObject>
#include <QSharedMemory>
#include <QQueue>
#include <QByteArray>
#include <QBuffer>
#include <QTimer>
#include <QDebug>

class  QueueShareMemory : public QObject
{

public:
    QueueShareMemory(QSharedMemory::AccessMode ModelOne , QSharedMemory::AccessMode ModelTwo , QObject *parent = 0);
    ~QueueShareMemory();
    void setSHMOneModel(QSharedMemory::AccessMode Model){SHMOneModel = Model;}
    void setSHMTwoModel(QSharedMemory::AccessMode Model){SHMTwoModel = Model;}
    bool getSHMData(QQueue<QByteArray> &QueuData);
    void writeSHMData(const QByteArray WriteData);

private:
    QQueue<QByteArray> mReadQueue;
    QQueue<QByteArray> mWriteQueue;
    QTimer mtWriteQueueTimer;
    QSharedMemory *mpSHMOne;
    QSharedMemory::AccessMode SHMOneModel;
    QSharedMemory *mpSHMTwo;
    QSharedMemory::AccessMode SHMTwoModel;

    void writeSHMDataTimerEvent();
    void readSHMDataTimerEvent();
};

#endif // QUEUESHAREMEMORY_H
