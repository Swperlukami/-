#ifndef APCONTROLLER_H
#define APCONTROLLER_H

#include <QObject>

class APController : public QObject
{
    Q_OBJECT
public:
    explicit APController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // APCONTROLLER_H