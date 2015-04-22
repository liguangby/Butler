#include "changeuserinfor.h"
#include "ui_changeuserinfor.h"
#include <QDebug>
Changeuserinfor::Changeuserinfor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Changeuserinfor)
{
    ui->setupUi(this);
    connect(&timer,SIGNAL(timeout()),this,SLOT(usernameset()));
    changeword = user_change.initDatabase("User_registdata.db");


}

Changeuserinfor::~Changeuserinfor()
{
    delete ui;
}

void Changeuserinfor::usernameset()
{
    ui->username->setText(userchangeinforname);
    timer.stop();
}

void Changeuserinfor::on_pushButton_clicked()
{
    if(ui->password->text()==NULL)
    {
        QMessageBox::information(this,"提示","密码不能为空",QMessageBox::Yes);

    }
    else
    {
        QSqlQuery query(changeword);
        QString sql = "select * from User_registdata where User_name = '";
        sql += userchangeinforname;
        sql +="'";
        //"create table User_registdata (User_password varch(30),User_phone varch(15),User_email varch(40),User_self varch(50),User_self_password varch(50),User_reg_time varch(20))");
        QString update = "UPDATE User_registdata SET User_password = '";
        update += ui->password->text();
        update += "', User_phone ='";
        update += ui->Phone->text();
        update += "',User_email = '";
        update += ui->Email->text();
        update +="' where User_name = '";
        update += userchangeinforname;
        update +="'";
        query.exec(sql);
        query.exec(update);
        this->close();
    }
}

void Changeuserinfor::on_pass_editingFinished()
{
    if(ui->pass->text()!=ui->password->text())
    {
       QMessageBox::information(this,"提示","两次输入不匹配",QMessageBox::Yes);
    }
}
