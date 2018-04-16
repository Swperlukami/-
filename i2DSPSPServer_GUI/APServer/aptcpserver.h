#ifndef APTCPSERVER_H
#define APTCPSERVER_H

#include "tcpsocket.h"
#include <QObject>
#include <QTcpServer>


class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = NULL);
    bool startTcpServer(QString Host , quint16 Port);
    bool stopTcpServer();

public slots:
    void processUISetAPEvent(QByteArray UIEvent);

    void setAPInputNo(QString APIP , int InputNo);
    void setAPEmgMode(QString APIp , bool isEmgMode);
    void apSeltTest();

protected:
    void incomingConnection(qintptr socketDescriptor);

private slots:
    void disConneted(qintptr socketDescriptor);

signals:
    void serverAPEvent(QByteArray event);

private:
    TcpSocket *mpTcpSocket;
    QHash<qintptr , TcpSocket*>mClientSocketDescriptorList;



};

#endif // APTCPSERVER_H
