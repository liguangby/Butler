#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
class database
{
public:
    database();
    QSqlDatabase db;
    QSqlDatabase initDatabase(QString name);
    void closedb();
};

#endif // DATABASE_H
