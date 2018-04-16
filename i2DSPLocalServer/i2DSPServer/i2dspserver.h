#ifndef I2DSPSERVER_H
#define I2DSPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtGlobal>
#include "tcpsocket.h"
#include "public.h"

class i2DSPServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit i2DSPServer(QObject *parent = 0);
    bool startTcpServer(QString Host , quint16 Port);
    bool stopTcpServer();

    void incomingConnection(qintptr socketDescriptor);

signals:

public slots:
    void disConneted(qintptr socketDescriptor);


private:
    QTcpServer mServer;

    TcpSocket *mpTcpSocket;
    QHash<qintptr , TcpSocket*>mClientSocketDescriptorList;

};

#endif // I2DSPSERVER_H
