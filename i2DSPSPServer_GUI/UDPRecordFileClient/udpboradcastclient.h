#ifndef UDPBORADCASTCLIENT_H
#define UDPBORADCASTCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QDateTime>
#include <QNetworkDatagram>
#include <QNetworkInterface>
class UDPBoradcastClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPBoradcastClient(QObject *parent = nullptr);
    bool bindBroadcastPort(int port);
    void startAndStopBroadcast(bool isStart);
signals:

public slots:
private:
    QUdpSocket m_BroadcastUdpSocket;
    QFile m_RecordFile;
    void recordBroadcastFile();
};

#endif // UDPBORADCASTCLIENT_H
