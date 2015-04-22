#include "boot.h"
#include "ui_boot.h"
#include <QDebug>
Boot::Boot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Boot)
{
    ui->setupUi(this);
    SETICON;
    QTimer::singleShot(1,this,SLOT(myloginrecode()));//解决了，哈哈
   /***************实用movie播放GIf动画*****************************/
    movie = new QMovie("://sources/login.gif");
    ui->background->setMovie(movie);
   /***************************************************************/
   thetip = true;     //标志位
   timertip = new QTimer(this);
   connect(timertip,SIGNAL(timeout()),this,SLOT(stopmovie()));
}
void Boot::keyPressEvent(QKeyEvent *)      //哈哈，在这里监听键盘，如果有键按下，这停止播放动画，这里不需要有半段是是否为制动登陆，因为如果是自动登陆，就不会播放画面
{                                                       //我草。悲剧了，这尼玛又开始启动双登录端了
    stopmovie();                                         //解决，不能使用signaletimer。不然没法停止，虽然简单
}

void Boot::myloginrecode()     //这逻辑真尼玛操蛋，一下午就搞了一个这个东西
{
    user_recode = dbopen.initDatabase("User_recode.db");
    QSqlQuery query;
    query.exec("select * from User_login");
    while (query.next())
    {
        if(query.value(2).toString()=="true")
        {
            thetip= false;
            QDateTime userlogin;
            QTime time;
            QDate data;
            userlogin.setDate(data.currentDate());
            userlogin.setTime(time.currentTime());
            QString userlogintime = userlogin.toString("yyyyMMddhhmmss");
            QString userloginname = query.value(0).toString();
            QString userloginpassword = query.value(1).toString();
            user_recode = dbopen.initDatabase("User_recode.db");
            QSqlQuery query(user_recode);
            QString sql = "insert into User_recode values('"+userlogintime+"','"+userloginname+"','"+userloginpassword+"','"+"自动登录系统"+"')";
            query.exec(sql);
            addact(userloginname);
            work.getdataname(userloginname);
            work.movepainter();
            movie->stop();
            this->close();
            w.close();
            work.show();//启动主工作区*注意生命周期
            dbopen.closedb();
            break;
        }
    }
    if(thetip)
    {
        movie->start();
        timertip->start(10000);
        opacity = 1.0;
    }
}
void Boot::addact(QString userloginname)
{
    QString userdataname = "User_Data_";
    userdataname += userloginname;
    userdataname += ".db";
    user_data = dbopen.initDatabase(userdataname);
    QSqlQuery query(user_data);
    query.exec("create table UserData (User_data_number varch(20) primary key, Data_time datatime,Data_type varch(10),Data_services varch(30),Data_money int,Data_pay_type varch(10), Data_ramark varch(200))");
    dbopen.closedb();
}
//关闭动画，进入登录界面
void Boot::stopmovie()
{
    deffect = new QGraphicsOpacityEffect();
    deffect->setOpacity(opacity);
    this->setGraphicsEffect(deffect);
    //connect(&dtimer,SIGNAL(timeout()),this,SLOT(change()));     //淡出效果
    dtimer.stop();
    timertip->stop();
    movie->stop();
    this->close();
    w.exec();

}
//淡出函数    //没法解决在结束是白屏问题
//void Boot::change()
//{
//    if(opacity<=0.0)//判断如果全部透明以后，将定时器和窗口关闭，调出登录界面
//    {
//        dtimer.stop();
//        movie->stop();
//        this->close();
//        w.exec();
//    }
//    else //如果效果没有完全淡出，加强淡出效果
//    {
//        deffect->setOpacity(opacity);
//        ui->background->setGraphicsEffect(deffect);
//        opacity-=0.1;
//    }
//}

/*****************隐藏标题栏可以移动代码*********************/
void Boot::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}

void Boot::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    move(x()+dx, y()+dy);
}

void Boot::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    move(x()+dx, y()+dy);
}
/***********************************************/
Boot::~Boot()
{
    delete ui;
}
