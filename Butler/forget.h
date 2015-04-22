#ifndef FORGET_H
#define FORGET_H
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database.h"
#include <QMessageBox>
namespace Ui {
class Forget;
}

class Forget : public QDialog
{
    Q_OBJECT

public:
    explicit Forget(QWidget *parent = 0);
    ~Forget();


private slots:
    void on_enter_clicked();

    void on_user_name_textChanged(const QString &arg1);



    void on_user_name_editingFinished();

private:
    Ui::Forget *ui;
    QSqlDatabase forgetpassword; //新建数据库实体类
    database user_forget;  //新建数据库
};

#endif // FORGET_H
