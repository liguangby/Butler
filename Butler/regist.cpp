#include "regist.h"
#include "ui_regist.h"
Regist::Regist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Regist)
{
    ui->setupUi(this);
    SETICON;
    /*************背景动画***********/
    movie = new QMovie(":/sources/regist.gif");
    ui->user_background->setMovie(movie);
    movie->start();
    deffect = new QGraphicsOpacityEffect();
    deffect->setOpacity(0.8);
    ui->label->setStyleSheet("QLabel{border-image:url(:/regist/images/user/user_regist_background.png);}");
    ui->label->setGraphicsEffect(deffect);
    /**********************数据库***************************/
    user_registdata = registopen.initDatabase("User_registdata.db");
    QSqlQuery registquery(user_registdata);
    registquery.exec("create table User_registdata (User_name varch(30) primary key,User_password varch(30),User_phone varch(15),User_email varch(40),User_self varch(50),User_self_password varch(50),User_reg_time varch(20))");
    /**********************系统美化*******************/
    ui->user_name->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_name.png);}");
    ui->user_password_0->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_password.png);}");
    ui->user_password_1->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_password.png);}");
    ui->user_phone->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_phone.png);}");
    ui->user_email->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_email.png);}");
    ui->user_self_password->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_pass.png);}");
    ui->user_self_in->setStyleSheet("QLineEdit{border-image:url(:/regist/images/user/user_question.png);}");
    ui->user_save->setStyleSheet("QPushButton{border-image:url(:/regist/images/user/user_regist_ok_0.png);}QPushButton:hover{border-image:url(:/regist/images/user/user_regist_ok_1.png);}QPushButton:pressed{border-image:url(:/regist/images/user/user_regist_ok_0.png);}");
    ui->user_cancle->setStyleSheet("QPushButton{border-image:url(:/regist/images/user/user_regist_cancle_0.png);}QPushButton:hover{border-image:url(:/regist/images/user/user_regist_cancle_1.png);}QPushButton:pressed{border-image:url(:/regist/images/user/user_regist_cancle_0.png);}");
}
/*****************隐藏标题栏可以移动代码*********************/
void Regist::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}

void Regist::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    move(x()+dx, y()+dy);
}

void Regist::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    move(x()+dx, y()+dy);
}
/***********************************************/
Regist::~Regist()
{
    delete ui;
}
//关闭注册界面
void Regist::on_user_cancle_clicked()
{
    this->close();
    movie->stop();
}
//将预设问题显示到编辑栏
void Regist::on_user_self_yu_currentIndexChanged(const QString &arg1)
{
    ui->user_self_in->setText(arg1);
}
//保存数据
void Regist::on_user_save_clicked()
{
    if(ui->user_name->text()==NULL)
    {
        QMessageBox::information(this,"警告","用户名不能为空",QMessageBox::Yes);
    }
    else
    {
        if((ui->user_password_0->text())!=(ui->user_password_1->text()))
        {
            QMessageBox::information(NULL,"提示","密码不匹配,请重新输入！",QMessageBox::Yes);
        }
        else
        {
            if(compaer_usermessage())
            {
                QMessageBox::information(this,"警告","用户名已存在！",QMessageBox::Yes);
            }
            else
            {
                judeg_information();
            }
        }
    }



}
//判定函数
void Regist::judeg_information()
{
    if((ui->user_phone->text()==NULL)||(ui->user_email->text()==NULL)||(ui->user_self_in->text()==NULL)||(ui->user_self_password->text()==NULL))
    {
        int flag=QMessageBox::information(this,"提示","信息不完整，确认储存吗？",QMessageBox::Yes|QMessageBox::No);
        switch (flag) {
        case QMessageBox::Yes:
            insertuser_information();
            this->close();
            break;
        case QMessageBox::No:
            break;
        default:
            break;
        }
    }
    else
    {
        insertuser_information();
        this->close();
    }
}
//查重函数
bool Regist::compaer_usermessage()
{
    user_registdata = registopen.initDatabase("User_registdata.db");
    QSqlQuery usermessage(user_registdata);
    usermessage.exec("select * from User_registdata");
    while (usermessage.next())
    {
        if(usermessage.value(0)==(ui->user_name->text()))
        {
            return true;

            break;
        }

    }
    return false;

}
//添加信息函数
void Regist::insertuser_information()
{
    QString user_name,user_password,user_phone,user_email,user_question,user_qusetion_password,user_reg_time;
    user_password = ui->user_password_0->text();
    user_name=ui->user_name->text();
    user_phone = ui->user_phone->text();
    user_email = ui->user_email->text();
    user_question = ui->user_self_in->text();
    user_qusetion_password = ui->user_self_password->text();
    QDateTime userregist;
    QTime time;
    QDate data;
    userregist.setDate(data.currentDate());
    userregist.setTime(time.currentTime());
    QString userregisttime = userregist.toString("yyyyMMddhhmmss");
    user_reg_time = userregisttime;
    QString sql = "insert into User_registdata values('";
    sql += user_name;
    sql += "','";
    sql += user_password;
    sql += "','";
    sql += user_phone;
    sql += "','";
    sql += user_email;
    sql += "','";
    sql += user_question;
    sql += "','";
    sql += user_qusetion_password;
    sql += "','";
    sql += userregisttime;
    sql += "')";
    user_registdata = registopen.initDatabase("User_registdata.db");
    QSqlQuery registquery(user_registdata);
    registquery.exec(sql);
    QMessageBox::information(this,"提示","注册成功",QMessageBox::Yes);
    registopen.closedb();



//    QString userdataname = "User_Data_";
//    userdataname += user_name;
//    userdataname += ".db";
//    user_data = registopen.initDatabase(userdataname);
//    QSqlQuery query(user_data);
//    query.exec("create table UserData (User_data_number varch(20) primary key, Data_time datatime,Data_type varch(10),Data_services varch(30),Data_money int,Data_pay_type varch(10), Data_ramark varch(200))");
//    registopen.closedb();


}
//随时检测用户名是否存在
void Regist::on_user_name_editingFinished()
{
    user_registdata = registopen.initDatabase("User_registdata.db");
    QSqlQuery query(user_registdata);
    query.exec("select * from User_registdata");
    QString user_name=ui->user_name->text();
    while (query.next())
    {
        if(query.value(0).toString()==user_name)
        {
            QString data= "用户：";
            data += user_name;
            data += " 已存在";
            QMessageBox::information(this,"提示",data,QMessageBox::Yes);
        }
    }
    registopen.closedb();
}

//检测用户名首字母是否为数字
void Regist::on_user_name_textChanged(const QString &arg1)
{
    if(arg1!=NULL)
    {
        if((arg1.mid(0,1)=="0")||(arg1.mid(0,1)=="1")||(arg1.mid(0,1)=="2")||(arg1.mid(0,1)=="3")||(arg1.mid(0,1)=="4")||(arg1.mid(0,1)=="5")||(arg1.mid(0,1)=="6")||(arg1.mid(0,1)=="7")||(arg1.mid(0,1)=="8")||(arg1.mid(0,1)=="9"))
        {
            QMessageBox::information(this,"提示","用户名首字母不能为数字",QMessageBox::Yes);
            if(QMessageBox::Yes)
            {
                ui->user_name->clear();
            }
        }
    }
}
//密码长度判定
void Regist::on_user_password_0_editingFinished()
{
    int theuserpasswordlength=ui->user_password_0->text().length();
    if(theuserpasswordlength<6&&theuserpasswordlength!=0)
    {
        QMessageBox::information(this,"提示","密码过于简单，请重新设置，推荐为数字和字母组合！",QMessageBox::Yes);

    }
}
