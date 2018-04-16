#ifndef SELECTSERIALPORT_H
#define SELECTSERIALPORT_H

#include <QObject>

class SelectSerialPort : public QSerialPort
{
public:
    SelectSerialPort();
};

#endif // SELECTSERIALPORT_H