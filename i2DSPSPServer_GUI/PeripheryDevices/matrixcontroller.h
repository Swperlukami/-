#ifndef MARTIXCONTROLLER_H
#define MARTIXCONTROLLER_H

#include "tcpsocketbasecontroller.h"
#include "matrix_protocol.h"
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QApplication>

class MatrixController : public TcpSocketBaseController
{
    Q_OBJECT
public:
    explicit MatrixController(QObject *parent = nullptr);

public slots:
    void userSetMatrixEvent(QString cmd , int channelNum , QString songName , int vol);
    bool syncMatrixAudioFiles();
signals:
    void matrixEvents(QString cmdType , QVariant params = 0);

private slots:

    void matrixConneted();
    void matrixDisconnted();
private:
      void processData(QByteArray data);

      bool isDiskMountOK;
      QTimer m_checkSDDirTimer;

      bool checkSDDirExist() ;
};

#endif // MARTIXCONTROLLER_H
