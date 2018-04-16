#include <QCoreApplication>
#include <QThread>
#include "public.h"
#include "i2DSPServer/i2dspserver.h"

QueueShareMemory *gp_SHM = NULL;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(!connect_database())
        return 0;
    if(!init_database())
        return 0;
    gp_SHM = new QueueShareMemory(QSharedMemory::ReadOnly , QSharedMemory::ReadWrite);
    if(gp_SHM == NULL)
        return 0;

//    gp_SHM->writeSHMData(QByteArray("I am Server 1"));
//    gp_SHM->writeSHMData(QByteArray("I am Server 2"));
//    gp_SHM->writeSHMData(QByteArray("I am Server 3"));
//    gp_SHM->writeSHMData(QByteArray("I am Server 4"));
    i2DSPServer Server;
   if(!Server.startTcpServer("192.168.2.50" , 6010))
       return 0;

    return a.exec();
}
