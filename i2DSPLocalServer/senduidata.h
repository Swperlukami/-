#ifndef SENDUIDATA_H
#define SENDUIDATA_H
#include "mc_uiprotocol.h"
#include <QString>
#include <QByteArray>

void lt_on_off_line_(QString ltisn , bool isonline);
void sub_dev_event(QString subdevisn , QString ltisn, int devtype, int port , int cmd);

#endif // SENDUIDATA_H
