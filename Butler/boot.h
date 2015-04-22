#ifndef BOOT_H
#define BOOT_H

#include <login.h>
#include <QDialog>
#include <QMouseEvent>
#include <QMovie>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include "mainwindow.h"
#include "definelist.h"
#include <QKeyEvent>
namespace Ui {
class Boot;
}

class Boot : public QDialog
{
    Q_OBJECT

public:
    explicit Boot(QWidget *parent = 0);
    void addact(QString userloginname);
    ~Boot();

protected://鼠标拖动函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *);
private slots:
    void stopmovie(); //停止槽函数
    void myloginrecode();
//    void change();//渐变槽函数
private:
    Ui::Boot *ui;
    QPoint last;//隐藏标题栏拖动函数
    QMovie *movie;//动画参数
    QGraphicsOpacityEffect *deffect;//淡出参数
    float opacity;//淡出参数
    QTimer dtimer,*timertip;//淡出定时器
    Login w;// 登录

    QSqlDatabase user_recode,user_data;//用户记录
    database dbopen;//用户登录表
    MainWindow work;
    bool thetip,thetip1;


};

#endif // BOOT_H
