#include "database.h"
#include <QMessageBox>
#include <QSqlQuery>
database::database()//注意多数据库的打开方式
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db=QSqlDatabase::addDatabase("QSQLITE");

}
QSqlDatabase database::initDatabase(QString name)
{

    db.setDatabaseName(name);

    if(!db.open())
    {
        QMessageBox msgBox;
        msgBox.setText("文件打开错误，请重新尝试。");
    }
    return db;
}

void database::closedb()
{
    db.close();
}
