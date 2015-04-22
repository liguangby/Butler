#ifndef REGIST_H
#define REGIST_H

#include "definelist.h"
#include <QDialog>
#include <QMouseEvent>
#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMovie>
#include <QMessageBox>
#include <QDateTime>
#include <QGraphicsOpacityEffect>
namespace Ui {
class Regist;
}

class Regist : public QDialog
{
    Q_OBJECT
protected: //鼠标拖动函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool compaer_usermessage();//查重函数
    void insertuser_information();//添加信息函数
    void judeg_information(); //判断函数
public:
    explicit Regist(QWidget *parent = 0);
    ~Regist();

private slots:
    void on_user_cancle_clicked();

    void on_user_self_yu_currentIndexChanged(const QString &arg1);

    void on_user_save_clicked();

    void on_user_name_editingFinished();

    void on_user_name_textChanged(const QString &arg1);

    void on_user_password_0_editingFinished();

private:
    Ui::Regist *ui;
    QPoint last;//隐藏标题栏拖动函数
    QSqlDatabase user_registdata,user_data;//用户注册信息
    database registopen;//用户注册
    QMovie *movie;//动画参数
    QGraphicsOpacityEffect *deffect;//透明度

};

#endif // REGIST_H
