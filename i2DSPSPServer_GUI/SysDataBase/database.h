#ifndef DATABASE_H
#define DATABASE_H
#include <QHostAddress>
#include <QDebug>
#include <QHash>
#include <QtGlobal>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>

#define FLINE __FILE__<<"Line:"<<__LINE__


bool connect_database();
bool exec_database(QString statement);
bool query_database(QString statement, QSqlQuery &query);

bool get_floor_devices(int devtype , int buildno , int floorno , QSqlQuery &query);

void init_test_database();
void delete_database();
#endif // DATABASE_H
