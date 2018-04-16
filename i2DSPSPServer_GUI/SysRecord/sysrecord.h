#ifndef SYSRECORD_H
#define SYSRECORD_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QString>
#include <QDebug>

class SysRecord : public QObject
{
    Q_OBJECT
public:
    explicit SysRecord(QObject *parent = nullptr);

    static void systemLogRcord(QString msg);

signals:

public slots:

};

#endif // SYSRECORD_H
