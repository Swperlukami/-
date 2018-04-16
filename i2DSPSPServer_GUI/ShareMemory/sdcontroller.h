#ifndef SDCONTROLLER_H
#define SDCONTROLLER_H

#include <QObject>

class SDController : public QObject
{
    Q_OBJECT
public:
    explicit SDController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SDCONTROLLER_H
