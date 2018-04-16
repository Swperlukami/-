#ifndef SYSTEMEVENTSCONTROLLER_H
#define SYSTEMEVENTSCONTROLLER_H

#include <QObject>

class SystemEventsController : public QObject
{
    Q_OBJECT
public:
    explicit SystemEventsController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SYSTEMEVENTSCONTROLLER_H