#include "login.h"
#include "ui_login.h"
#include <QDebug>
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    SETICON;
    remberpassword = false; //记住密码初始化，默认为否
    logintip = false;
    /********************更新时间**********************/
    timeid = new QTimer(this);
    connect(timeid,SIGNAL(timeout()),this,SLOT(timedate()));
    timeid->start(100);
    /********************数据库********************/
    user_recode = dbopen.initDatabase("User_recode.db");//打开用户记录数据库
    QSqlQuery query;
    query.exec("create table User_recode (User_login_time varch(20) primary key,User_name varch(30),User_password varch(30),User_type varch(30))");
    dbopen.closedb();     //创建数据库，并关闭
    QTimer::singleShot(1,this,SLOT(myloginrecode()));//解决了，哈哈
    /**********************界面美化*************************/
    ui->login_background->setStyleSheet("QLabel{border-image:url(:/login/images/user_ligin.png);}");
    ui->enter->setStyleSheet("QPushButton{border-image:url(:/login/images/user_enter_0.png);}QPushButton:hover{border-image:url(:/login/images/user_enter_1.png);}QPushButton:pressed{border-image:url(:/login/images/user_enter_0.png);}");
    ui->user_name->setStyleSheet("QLineEdit{border-image:url(:/login/images/user_up.png);}");
    ui->user_password->setStyleSheet("QLineEdit{border-image:url(:/login/images/user_down.png);}");
    ui->background->setStyleSheet("QLabel{border-image:url(:/login/images/back.png);}");
    ui->close->setStyleSheet("QPushButton{border-image:url(:/login/images/user_exit_0.png);}QPushButton:hover{border-image:url(:/login/images/user_exit_1.png);}QPushButton:pressed{border-image:url(:/login/images/user_exit_0.png);}");
    ui->regist->setStyleSheet("QPushButton{border-image:url(:/login/images/user_regist_0.png);}QPushButton:hover{border-image:url(:/login/images/user_regist_1.png);}QPushButton:pressed{border-image:url(:/login/images/user_regist_0.png);}");
    ui->user1->setStyleSheet("QLabel{border-image:url(:/login/images/user_n.png);}");
    ui->user2->setStyleSheet("QLabel{border-image:url(:/login/images/uesr_p.png);}");
    ui->baby->setStyleSheet("QLabel{border-image:url(:/login/images/baby.png);}");
    ui->ferget->setStyleSheet("QPushButton{background-color:transparent}");//设置背景透明
    ui->close1->setStyleSheet("QPushButton{border-image:url(:/images/close_0.png);}QPushButton:hover{border-image:url(:/images/close_1.png);}QPushButton:pressed{border-image:url(:/images/close_0.png);}");
    /**************************************************/
}
//初始化记录
void Login::myloginrecode()
{
    user_recode = dbopen.initDatabase("User_recode.db");
    QSqlQuery query;
    query.exec("select * from User_login");
    while (query.last())
    {
        ui->user_name->setText(query.value(0).toString());
        ui->user_password->setText(query.value(1).toString());
        ui->remember->setChecked(true);
        ui->automatic->setChecked(false);
        remberpassword = true;
        dbopen.closedb();
        break;
    }
}
//添加用户登陆信息  //并创建用户数据库
void Login::addact(QString userloginname)
{
    QString userdataname = "User_Data_";
    userdataname += userloginname;
    userdataname += ".db";
    user_data = dbopen.initDatabase(userdataname);
    QSqlQuery query(user_data);
    query.exec("create table UserData (User_data_number varch(20) primary key, Data_time datatime,Data_type varch(10),Data_services varch(30),Data_money int,Data_pay_type varch(10), Data_ramark varch(200))");
    dbopen.closedb();

}

//更新时间函数
void Login::timedate()
{
    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    QString currentDate = dt.toString("yyyy年MM月dd日hh时mm分ss秒");
    ui->timedata->setText(currentDate);
    ui->timedata->setStyleSheet("QLabel{color:#CD0000;}");
}
/*****************隐藏标题栏可以移动代码*********************/
void Login::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}
void Login::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void Login::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    move(x()+dx, y()+dy);
}
/***********************************************************/
Login::~Login()
{
    delete ui;
    timeid->stop();
    delete timeid;
    dbopen.closedb();
}
//关闭按钮槽函数
void Login::on_close_clicked()
{
    this->close();
    user_recode.close();
}
//点击进入管理系统
void Login::on_enter_clicked()
{
    if(ui->user_name->text()==NULL)
    {
        QMessageBox::information(this,"提示","用户名不能为空",QMessageBox::Yes);

    }
    else
    {
        if(query_user()) //判断用户名是否存在
        {
            if(comp_user_password()) //判定密码是否正确
            {
                if(logintip) //自动登录
                {
                    automatic_login();
                }
                else if(remberpassword) //记住密码
                {
                    remberuserpassword();
                }
                else //普通登录
                {
                    rember_user_login();//记录用户信息
                }
                rember_user_login();//记录用户信息
            }
            else
            {
                QMessageBox::information(this,"提示","密码错误",QMessageBox::Yes);
            }
        }
        else
        {
            int flag =QMessageBox::information(this,"提示","用户不存在，是否注册？",QMessageBox::Yes|QMessageBox::No);
            switch (flag) {
                break;
            case QMessageBox::Yes:
                on_regist_clicked();
                break;
            case QMessageBox::No:
            default:
                break;
            }
        }
    }
}
//查询用户是否注册函数
bool Login::query_user()
{
    QSqlDatabase user_message_query;
    user_message_query = dbopen.initDatabase("User_registdata.db");
    QSqlQuery query(user_message_query);
    query.exec("select * from User_registdata");
    while (query.next())
    {

        if(query.value(0).toString()==(ui->user_name->text()))
        {
            dbopen.closedb();
            return true;
            break;
        }
    }
    dbopen.closedb();
    return false;
}
//判定密码是否正确函数
bool Login::comp_user_password()
{
    if(ui->user_password->text()==NULL)
    {
        return false;
    }
    QSqlDatabase user_message_query;
    user_message_query = dbopen.initDatabase("User_registdata.db");
    QSqlQuery query(user_message_query);
    query.exec("select * from User_registdata");
    while (query.next())
    {
        if(query.value(1).toString()==(ui->user_password->text()))
        {
            dbopen.closedb();
            return true;
            break;
        }
    }
    dbopen.closedb();
    return false;
}

//记录用户登录
void Login::rember_user_login()
{
    QDateTime userlogin;
    QTime time;
    QDate data;
    userlogin.setDate(data.currentDate());
    userlogin.setTime(time.currentTime());
    QString userlogintime = userlogin.toString("yyyyMMddhhmmss");
    QString userloginname = ui->user_name->text();
    QString userloginpassword = ui->user_password->text();
    user_recode = dbopen.initDatabase("User_recode.db");
    QSqlQuery query(user_recode);
    if(remberpassword)//判定是否记住密码
    {
        QString sql = "insert into User_recode values('";
        sql += userlogintime;
        sql += "','";
        sql += userloginname;
        sql += "','";
        sql += userloginpassword;
        sql += "','";
        sql += "登录系统";
        sql += "')";
        query.exec(sql);
        create_user_data();
        dbopen.closedb();
        work.getdataname(userloginname);
        work.movepaintergo();
        work.show();//启动主工作区*注意生命周期
        this->close();
 //       qDebug()<<userloginname<<"-------------------------------->>>";


    }
    else {   //不记住密码
        QString sql = "insert into User_recode values('";
        sql += userlogintime;
        sql += "','";
        sql += userloginname;
        sql += "','";
        sql += "','";
        sql += "登录系统";
        sql += "')";
        query.exec(sql);
        create_user_data();
        dbopen.closedb();
        work.getdataname(userloginname);
        work.movepaintergo();
        work.show();//启动主工作区*注意生命周期
        this->close();
        qDebug()<<userloginname<<"-------------------------------123->>>";
    }
//    ui->user_name->clear();
    ui->user_password->clear();
    dbopen.closedb();
}
//创建用户数据库
void Login::create_user_data()
{
    QString userdataname = "User_Data_";
    userdataname += ui->user_name->text();
    userdataname += ".db";
    user_data = dbopen.initDatabase(userdataname);
    QSqlQuery query(user_data);
    query.exec("create table UserData (User_data_number varch(20) primary key, Data_time datatime,Data_type varch(10),Data_services varch(30),Data_money int,Data_pay_type varch(10), Data_ramark varch(200))");
    dbopen.closedb();
}
//记住密码
void Login::on_remember_clicked(bool checked)
{
        remberpassword = checked;
        if(!checked)
        {
            ui->automatic->setChecked(false);
            remberuserpassword_no();
        }
}
//创建记住密码表
void Login::remberuserpassword()
{
    user_data = dbopen.initDatabase("User_recode.db");
    QSqlQuery query(user_data);
    query.exec("create table User_login(User_name varch(30) primary key,User_password varch(30),User_logintip varch(10))");  
    QString sql = "insert into User_login values('";
    sql += ui->user_name->text();
    sql += "','";
    sql += ui->user_password->text();
    sql += "','";
    sql += "false";
    sql += "')";
    query.exec(sql);
    dbopen.closedb();
}
//不记住密码
void Login::remberuserpassword_no()
{
    user_data = dbopen.initDatabase("User_recode.db");
    QSqlQuery query(user_data);
    query.exec("select * from User_login");
    while (query.next())
    {
        if(query.value(0).toString()==(ui->user_name->text()))
        {
            QString sql = "delete from User_login where User_name=";
            sql+="'";
            sql += ui->user_name->text();
            sql+="'";
            query.exec(sql);
        }
    }
    dbopen.closedb();
}
//自动登录
void Login::automatic_login()
{
    user_data = dbopen.initDatabase("User_recode.db");
    QSqlQuery query(user_data);
    query.exec("create table User_login(User_name varch(30) primary key,User_password varch(30),User_logintip varch(10))");
    query.exec("select * from User_login");
    while (query.next())
    {
        if((query.value(2).toString()=="true")&&(query.value(0).toString()!=(ui->user_name->text())))
        {
            QString user_changetip = "update User_login set User_logintip=";
            user_changetip += "'false' where User_name= ";
            user_changetip+="'";
            user_changetip += query.value(0).toString();
            user_changetip+="'";
            query.exec(user_changetip);
        }
        QString sql_updata ="update User_login set User_logintip=";
        sql_updata +="'true' where User_name= ";
        sql_updata +="'";
        sql_updata +=ui->user_name->text();
        sql_updata +="'";
        query.exec(sql_updata);
        QString sql = "insert into User_login values('";
        sql += ui->user_name->text();
        sql += "','";
        sql += ui->user_password->text();
        sql += "','";
        sql += "true";
        sql += "')";
        query.exec(sql);
        dbopen.closedb();
    }

}
//不记自动登录
void Login::automatic_login_no()
{
    user_data = dbopen.initDatabase("User_recode.db");
    QSqlQuery query(user_data);
    query.exec("select * from User_login");
    QString sql_updata ="update User_login set User_logintip=";
    sql_updata +="'false' where User_name= ";
    sql_updata +=ui->user_name->text();
    query.exec(sql_updata);
    dbopen.closedb();
}
//自动登录
void Login::on_automatic_clicked(bool checked)
{
    logintip = checked;
    if(checked)
    {
        ui->remember->setChecked(checked);
    }
    else {
        automatic_login_no();
    }
}
//注册按钮槽函数
void Login::on_regist_clicked()
{
    user_recode.close();
    Regist regist;
    regist.exec();
}
//忘记密码槽函数
void Login::on_ferget_clicked()
{
    Forget w;
    w.exec();
}
//自动填写
void Login::on_user_name_textChanged(const QString &arg1)
{
    user_data = dbopen.initDatabase("User_recode.db");
    QSqlQuery query(user_data);
    query.exec("select * from User_login");
    while (query.next())
    {
        if(query.value(0).toString()==arg1)
        {
            ui->user_password->setText(query.value(1).toString());
            ui->remember->setChecked(true);
            ui->automatic->setChecked(false);
            break;
        }
        else {
            ui->user_password->clear();
            ui->remember->setChecked(false);
            ui->automatic->setChecked(false);
        }
    }
    dbopen.closedb();
}

void Login::on_close1_clicked()
{
    this->close();
    user_recode.close();
}
