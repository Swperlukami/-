#ifndef POWERMODESERVER_H
#define POWERMODESERVER_H

#include <QObject>
#include <QByteArray>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include "matrixcontroller.h"
#include "powermodelcontroller.h"
#include "netmiccontroller.h"
#include "lisenlingspkcontroller.h"

#define FLINE __FILE__<<"Line:"<<__LINE__


class PowerModeServer : public QObject
{
    Q_OBJECT
public:
    explicit PowerModeServer(QObject *parent = nullptr);

    bool startServer(QString hostIp , int port);
    bool stopServer();
signals:

public slots:
    void powerModelClientConnect();
private:
    QTcpServer m_PowerModeServer;


public:
    MatrixController *m_pMatrixContrller;
    PowerModelController *m_pPowerModelConroller;
    NetMicController *m_pNetMicController;
    LisenlingSPKController *m_pListenSPKController;
};

#endif // POWERMODESERVER_H
