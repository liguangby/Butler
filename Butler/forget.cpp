#include "forget.h"
#include "ui_forget.h"

Forget::Forget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Forget)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/login/images/M.png"));//系统图标
    ui->user_name->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_name.png);}");
    ui->user_phone->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_phone.png);}");
    ui->user_email->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_email.png);}");
    ui->user_question->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_pass.png);}");
    ui->enter->setEnabled(false);


}
//析构函数
Forget::~Forget()
{
    delete ui;
}
//忘记密码
void Forget::on_enter_clicked()
{
    on_user_name_editingFinished();
    forgetpassword = user_forget.initDatabase("User_registdata.db");
    QSqlQuery query(forgetpassword);
    query.exec("select * from User_registdata");
    QString user_name,user_phone,user_email,user_question,user_password;
    user_name=ui->user_name->text();
    while (query.next())
    {
        if(query.value(0).toString()==user_name)
        {
            user_phone = query.value(2).toString();
            user_email = query.value(3).toString();
            user_question = query.value(4).toString();
            user_password = query.value(5).toString();
            if((ui->user_phone->text()==user_phone)&&(ui->user_email->text()==user_email)&&(ui->user_question->text()==user_password))
            {
                QString usermessage = "帐号：";
                usermessage +=ui->user_name->text();
                usermessage +="\n";
                usermessage += "密码：";
                usermessage += query.value(1).toString();
                int flag=QMessageBox::information(this,"提示",usermessage,QMessageBox::Yes);
                if(flag==QMessageBox::Yes)
                {
                    this->close();
                }
                break;
            }
            else
            {
                QMessageBox::information(this,"提示","用户信息错误！",QMessageBox::Yes);
                break;
            }
            break;
        }
    }
    user_forget.closedb();
}
//帐号监视
void Forget::on_user_name_textChanged(const QString &arg1)
{
    if(ui->user_name->text()!=NULL)
    {
        ui->enter->setEnabled(true);
    }
    forgetpassword = user_forget.initDatabase("User_registdata.db");
    QSqlQuery query(forgetpassword);
    query.exec("select * from User_registdata");
    while (query.next())
    {
        if(query.value(0).toString()==arg1)
        {
            ui->user_question->setText(query.value(4).toString());
        }
    }
    user_forget.closedb();
}
//帐号检测
void Forget::on_user_name_editingFinished()
{
    forgetpassword = user_forget.initDatabase("User_registdata.db");
    QSqlQuery query(forgetpassword);
    query.exec("select * from User_registdata");
    QString data = NULL;
    while (query.next())
    {
        if(query.value(0).toString()==ui->user_name->text())
        {
            data = query.value(0).toString();
        }
    }
    if(data == NULL)
    {
        QMessageBox::information(this,"提示","帐号不存在！",QMessageBox::Yes);
    }
    user_forget.closedb();
}
