#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "definelist.h"
#include "changeuserinfor.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database.h"
#include <QPoint>
#include <QMouseEvent>
#include "add_user_data.h"
#include <QFont>
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QProcess>
#include <QComboBox>
#include <QCalendarWidget>
#include <excelengine.h>
#include <QPainter>
#include <QBrush>
#include <QTime>
#include <QTimer>
#include <QHeaderView>
#include <QScrollBar>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected: //鼠标拖动函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void canclelogin();//取消自动登录
    void maketable();//制表
    void insertdata();//插入数据
    void addusershow();//动态添加
    void createActions();//鼠标事件
    void contextMenuEvent(QContextMenuEvent *event);//重载鼠标
    void deleteuserdata_t();
    void settablewidgetcolor();


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onHeaderCliked(int);

    void on_action_1_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_uaer_data_show_cellClicked(int row, int column);

    void adduserdata();

    void deleteuserdata();

    void reviseuserdata();

    void Sequence_time();

    void on_action_triggered();

    void on_action_out_triggered();

    void on_action_5_triggered();

    void on_action_4_triggered();

    void setcombox();

    void on_action_delete_triggered();

    void moneyupto();

    void moneydownto();

    void Sequence_type();

    void changemode();

    void paintEvent(QPaintEvent *);

    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_10_triggered();

    void on_theday_clicked();

    void on_themon_clicked();

    void on_theyear_clicked();

    void makeday();

    void timedate();

    void movepaintertrue();

    void on_cancalred_triggered();

    void on_time1_editingFinished();

    void on_time2_editingFinished();

    void on_select_clicked();

    void on_cancle_clicked();

    void on_changepassword_triggered();

public slots:

    void movepainter();

public:
    void getdataname(QString name);
    void movepaintergo();
private:
    Ui::MainWindow *ui;
    QPoint last;//隐藏标题栏拖动函数
    database dbopen;
    QSqlDatabase userdata;
    QString userdataname;//数据库名称
    QAction *action_add,*action_revise,*action_delete,*action_Sequence,*action_time,*action_type,*moneyup,*moneydown;  //鼠标右键
    QMenu *pop_menu,*sort_style;  //
    int the_row_number,the_row;

    QTableWidgetItem *item0 = NULL;
    QTableWidgetItem *item1 = NULL;
    QTableWidgetItem *item2 = NULL;
    QTableWidgetItem *item3 = NULL;
    QTableWidgetItem *item4 = NULL;
    QTableWidgetItem *item5 = NULL;
    QTableWidgetItem *item6 = NULL;

    QComboBox *combox=NULL,*combox1=NULL,*combox2=NULL;
    QLabel *lable,*lablename;
    double Min;
    QPainter *painterin,*painterout,*painter1,*painter2;
    QTimer *timeid;//获取当前时间
    QTimer *timer,*timer1;
    bool paixu;
    int they;
    bool colortip;
    QString userdatatime1,userdatatime2,userchangeinforname;


};

#endif // MAINWINDOW_H
