#ifndef LISENLINGSPKCONTROLLER_H
#define LISENLINGSPKCONTROLLER_H

#include <QObject>
#include <QSoundEffect>
#include "tcpsocketbasecontroller.h"
#include "SysDataBase/database.h"

class LisenlingSPKController : public TcpSocketBaseController
{
    Q_OBJECT
public:
    explicit LisenlingSPKController(QObject *parent = nullptr);

signals:
    void listenSPKEvents(QString cmdType , QVariant params = 0);

public slots:
    void userSetListenSPKEvent(QString cmdType , int vol = 0);

public slots:
    void listenSPKConnected();
    void listenSPKDisconnected();

private:
    void processData(QByteArray data);

   QSoundEffect *m_pListenSPKTestSound;
};

#endif // LISENLINGSPKCONTROLLER_H
