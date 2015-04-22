#ifndef CHANGEUSERINFOR_H
#define CHANGEUSERINFOR_H

#include <QDialog>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database.h"
#include <QMessageBox>
namespace Ui {
class Changeuserinfor;
}

class Changeuserinfor : public QDialog
{
    Q_OBJECT

public:
    explicit Changeuserinfor(QWidget *parent = 0);
    ~Changeuserinfor();
    QString userchangeinforname;
    QTimer timer;

private slots:
    void on_pushButton_clicked();

    void usernameset();

    void on_pass_editingFinished();

private:
    Ui::Changeuserinfor *ui;
    void uasenameset();
    QSqlDatabase changeword; //新建数据库实体类
    database user_change;  //新建数据库

};

#endif // CHANGEUSERINFOR_H
