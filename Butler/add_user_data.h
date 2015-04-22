#ifndef ADD_USER_DATA_H
#define ADD_USER_DATA_H

#include "definelist.h"
#include <QDialog>
#include <QDate>
#include <QDateTime>
#include <QTimer>
#include <QCalendarWidget>
#include <QMovie>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database.h"
#include <QValidator>
#include <QMessageBox>

namespace Ui {
class Add_user_data;
}

class Add_user_data : public QDialog
{
    Q_OBJECT

protected: //鼠标拖动函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void combobox_in();  //存入
    void combobox_out(); //支出
    void add_thedata_todata(); //加入数据库
public:
    explicit Add_user_data(QWidget *parent = 0);
    ~Add_user_data();

private slots:
    void timedate();

    void on_Add_cancle_clicked();

    void on_Add_save_clicked();

    void on_time_clicked();

    void setdata();

    void on_save_in_clicked(bool checked);

    void on_save_out_clicked(bool checked);

    void on_type_editTextChanged(const QString &arg1);

    void on_Add_money_textChanged(const QString &arg1);

    void on_close_clicked();

    void on_money_type_currentTextChanged(const QString &arg1);



private:
    Ui::Add_user_data *ui;
    QTimer *timeid;//获取当前时间
    QString currentDate;//记录号
    QCalendarWidget *calen;//日历
    QMovie *movie;//动画参数
    QPoint last;//隐藏标题栏拖动函数
    bool tag1;//收支标志
    database dbopen; //打开数据库
    QSqlDatabase userdata;//新建数据库
    QString userdataname;//用户名
    //********用户数据*********//
    QString user_add_number,user_add_time,user_add_what,user_add_money,uesr_add_type,user_add_remark,user_add_tip;
    //************************//
};

#endif // ADD_USER_DATA_H
