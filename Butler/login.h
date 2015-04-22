#ifndef LOGIN_H
#define LOGIN_H

#include "definelist.h"
#include <QDialog>
#include <QPoint>
#include <QMouseEvent>
#include <QDateTime>
#include <QTimer>
#include "database.h"
#include "regist.h"
#include "mainwindow.h"
#include "forget.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QApplication>
#include <QMessageBox>
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
protected: //鼠标拖动函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void create_user_data(); //创建用户数据库
    void rember_user_login(); //添加用户登录记录
    bool query_user(); //查询用户是否注册
    bool comp_user_password();//判定用户密码是否与注册是一致
    void remberuserpassword();//记住密码
    void automatic_login();//自动登录
    void automatic_login_no();//不自动登录
    void remberuserpassword_no();//不记住密码
    void addact(QString userloginname);//自动登录


private slots:
    void on_close_clicked();

    void timedate();//更新时间

    void on_enter_clicked();

    void on_remember_clicked(bool checked);

    void on_regist_clicked();

    void on_ferget_clicked();

    void on_automatic_clicked(bool checked);

    void on_user_name_textChanged(const QString &arg1);

    void on_close1_clicked();
public slots:
    void myloginrecode(); //登录记录初始化

private:
    Ui::Login *ui;
    QPoint last;//隐藏标题栏拖动函数
    QTimer *timeid;//获取当前时间
    QSqlDatabase user_recode,user_data;//用户记录
    database dbopen;//用户登录表
    bool remberpassword,logintip;//记住密码、自动登录
    MainWindow work; ///主工作区
public:



};

#endif // LOGIN_H
