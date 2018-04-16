#ifndef EMGBROADCASTDATABASE_H
#define EMGBROADCASTDATABASE_H
#include "database.h"


bool connect_broadcast_database();
bool exec_broadcast_database(QString statement);
bool query_broadcast_database(QString statement, QSqlQuery &query);


#endif // EMGBROADCASTDATABASE_H
