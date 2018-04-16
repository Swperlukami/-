#ifndef POWERMODESERVER_H
#define POWERMODESERVER_H

#include <QObject>

class PowerModeServer : public QObject
{
    Q_OBJECT
public:
    explicit PowerModeServer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // POWERMODESERVER_H