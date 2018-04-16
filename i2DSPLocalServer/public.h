#ifndef PUBLIC_H
#define PUBLIC_H
#include <QHostAddress>
#include <QDebug>
#include <QHash>
#include <QtGlobal>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QFile>
#include <QDir>
#include "ShareMemory/queuesharememory.h"

#define FLINE __FILE__<<"Line:"<<__LINE__

void get_mc_info();
bool connect_database();
bool init_database();
bool query_database(QString statement , QSqlQuery &query);
bool exec_database(QString statement);

int get_device_emglv(QString Ltid , int Port);
QString get_device_isn_by_ltid_and_port(QString Ltid , int Port);

void sys_device_event_sql_reord(QString id , int devType , int eventType);
void sys_device_info_record(QString msg);

#endif // PUBLIC_H
