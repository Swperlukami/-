#ifndef UDPBORADCASTCLIENT_H
#define UDPBORADCASTCLIENT_H

#include <QObject>

class UDPBoradcastClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPBoradcastClient(QObject *parent = nullptr);

signals:

public slots:
};

#endif // UDPBORADCASTCLIENT_H