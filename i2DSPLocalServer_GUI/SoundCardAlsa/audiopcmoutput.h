#ifndef AUDIOPCMOUTPUT_H
#define AUDIOPCMOUTPUT_H

#include <QObject>

class AudioPCMOutPut : public QObject
{
    Q_OBJECT
public:
    explicit AudioPCMOutPut(QObject *parent = nullptr);

signals:

public slots:
};

#endif // AUDIOPCMOUTPUT_H