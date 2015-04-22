#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SETICON;
    ui->uaer_data_show->setStyleSheet("QTableWidget{background-color:transparent}");
    maketable();     //创建表
    createActions();   //创建菜单
    /***********绘制图形***************///ui->widget   //this
    painterin = new QPainter(ui->widget);
    painterout = new QPainter(ui->widget);
    painter1 =  new QPainter(ui->widget);
    painter2 =  new QPainter(ui->widget);
    /**********************************/
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(makeday()));     //默认使用绘制当天消费的统计图，当用户显示当月，当年，总计的统计.5，秒后恢复到当天
    timer->start(5000);
    timeid = new QTimer(this);
    connect(timeid,SIGNAL(timeout()),this,SLOT(timedate()));       //获取本地时间
    timeid->start(100);
    lable = new QLabel(this);
    ui->statusbar->addWidget(lable);
    lablename = new QLabel(this);
    ui->statusbar->addWidget(lablename);     //向statusbar添加label控件
    connect(ui->uaer_data_show->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(onHeaderCliked(int)));    //表头排序连接
    paixu =true;     //表头金额升序降序排列标志位
    timer1 = new QTimer(this);//移动dimer
    connect(timer1,SIGNAL(timeout()),this,SLOT(movepaintertrue()));    //槽函数
    they = 250;    //起始位置
    colortip = false;     //红色标记标志位    默认为不标记红色支出
    //初始化查询时间
    ui->time1->setDateTime(QDateTime::currentDateTime());
    ui->time2->setDateTime(QDateTime::currentDateTime());
    ui->cancle->setEnabled(false);
    ui->moneyedit1->setText("0.0");
    ui->moneyedit2->setText("0.0");
    ui->allmonetin->setText("0.0");
    ui->allmoneyout->setText("0.0");
    ui->radioButton3->setChecked(true);
    userchangeinforname = "";


}
//为了调用移动函数
void MainWindow::movepaintergo()
{
    QTimer::singleShot(10,this,SLOT(movepainter()));   //停止10毫秒后开始移动绘图
}

void MainWindow::movepainter()    //再一次调用移动函数
{
     timer1->start(25);
}
void MainWindow::movepaintertrue()    //这尼玛才是真的移动函数
{
    ui->widget->move(750,they);
    they = they -10;
    if(they == -20)
    {
        timer1->stop();
    }
    makeday();      //这个其实是没有什么用的，只是为了消除下面的小红点
}

//  表头排序
void MainWindow::onHeaderCliked(int x)
{
    if(x == 1)
    {
        Sequence_time();   //单击表头按时间排序
    }
    else if(x ==2)
    {
        Sequence_type();    //单击表头按收支类型排序
    }
    else if(x == 4)     //单击表头按金额排序
    {
        if(paixu)
        {
            moneyupto();    //金额升序排序
            paixu = false;
        }
        else
        {
            moneydownto();   //金额降序排列
            paixu = true;
        }
    }
    else if(x == 6)
    {
        on_uaer_data_show_cellClicked(5, 2);     //解决单行数据修改无法保存问题//哈哈，只需要重新调用，写入假的坐标就好//妈妈再也不用担心了
    }
}

//更新时间函数
void MainWindow::timedate()
{
    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    QString currentDate = dt.toString("当前时间：yyyy年MM月dd日hh时mm分   ");
    QFont font;
    font.setBold(true);
    font.setPointSize(11);
    lable->setFont(font);
    lable->setText(currentDate);
}
//获取用户名
void MainWindow::getdataname(QString name)     //在statusbar显示信息
{
    userchangeinforname = name;
    userdataname = "User_Data_";
    userdataname += name;
    userdataname +=".db";
    insertdata();    //调用数据库插入函数
    QString username = "当前登陆用户： ";
    username += name;
    username += "     数据库：";
    username += userdataname;
    lablename->setText(username);
    QFont font;
    font.setBold(true);
    font.setPointSize(11);
    lablename->setFont(font);
}
//创建表
void MainWindow::maketable()
{
    ui->uaer_data_show->setSelectionBehavior(QAbstractItemView::SelectRows);  //选中整行
    ui->uaer_data_show->verticalHeader()->hide();//隐藏行号
    ui->uaer_data_show->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不可编辑
    ui->uaer_data_show->setColumnCount(7); //设置显示7列
    QStringList header;
    header << tr("记录号")<< tr("时间")<< tr("收支类型")<< tr("事物")<< tr("金额")<< tr("支付方式")<< tr("备注");
    ui->uaer_data_show->setHorizontalHeaderLabels(header);  //设计表头
    QFont font;
    font.setBold(false);    //设置粗体
    font.setPointSize(9);

    ui->uaer_data_show->horizontalHeader()->setFont(font);  //这是表头为粗体
    ui->uaer_data_show->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //列自动分配
    ui->uaer_data_show->setFrameShape(QFrame::NoFrame);  //设置不现实边框
    ui->uaer_data_show->setShowGrid(false); //设置不显示表格
    ui->uaer_data_show->horizontalHeader()->setHighlightSections(false);//防止单行塌陷

}
//插入信息
void MainWindow::insertdata()
{
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    query.exec("select * from UserData");
    while (query.next())
    {
        int row_cout = ui->uaer_data_show->rowCount();
        //ui->uaer_data_show->setBackgroundRole(QColor(255,0,0));
        ui->uaer_data_show->insertRow(row_cout);
        ui->uaer_data_show->setItem(row_cout,0,new QTableWidgetItem(query.value(0).toString()));
        ui->uaer_data_show->setItem(row_cout,1,new QTableWidgetItem(query.value(1).toString()));
        ui->uaer_data_show->setItem(row_cout,2,new QTableWidgetItem(query.value(2).toString()));
        ui->uaer_data_show->setItem(row_cout,3,new QTableWidgetItem(query.value(3).toString()));
        ui->uaer_data_show->setItem(row_cout,4,new QTableWidgetItem(query.value(4).toString()));
        ui->uaer_data_show->setItem(row_cout,5,new QTableWidgetItem(query.value(5).toString()));
        ui->uaer_data_show->setItem(row_cout,6,new QTableWidgetItem(query.value(6).toString()));
    }

    if((ui->uaer_data_show->rowCount())==0)      //判断是否有数据存在，如果没有，则将部分按钮设置为不可用
    {
        ui->action->setEnabled(false);
        ui->action_delete->setEnabled(false);
        ui->action_out->setEnabled(false);
        ui->action_7->setEnabled(false);
        ui->action_8->setEnabled(false);
        ui->action_9->setEnabled(false);
        ui->action_10->setEnabled(false);
    }
    if(ui->uaer_data_show->rowCount()<2)     //注意这是的rowcount ，是+1的
    {
        ui->action_7->setEnabled(false);
        ui->action_8->setEnabled(false);
        ui->action_9->setEnabled(false);
        ui->action_10->setEnabled(false);
    }
    ui->uaer_data_show->show();
    dbopen.closedb();
    settablewidgetcolor();
    makeday();
    if(ui->uaer_data_show->rowCount()>13)
    {
        int max =ui->uaer_data_show->rowCount()+2;
        ui->uaer_data_show->verticalScrollBar()->setMaximum(max);
        ui->uaer_data_show->verticalScrollBar()->setValue(max);
    }
    else
    {
        ui->uaer_data_show->verticalScrollBar()->setMaximum(13);
    }
}

/*****************隐藏标题栏可以移动代码*********************/
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    move(x()+dx, y()+dy);
}
/***********************************************************/
//析构函数
MainWindow::~MainWindow()
{
    userdataname.clear();
    delete ui;
}
//添加
void MainWindow::on_action_1_triggered()
{
    Add_user_data useradd;
    if(useradd.exec()==QDialog::Accepted)   //界面交互
    {
        addusershow();
        this->update();
    }
}
//动态添加
void MainWindow::addusershow()
{
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    query.exec("select * from UserData");
    while (query.last())
    {
        int row_cout = ui->uaer_data_show->rowCount();
        ui->uaer_data_show->insertRow(row_cout);
        ui->uaer_data_show->setItem(row_cout,0,new QTableWidgetItem(query.value(0).toString()));
        ui->uaer_data_show->setItem(row_cout,1,new QTableWidgetItem(query.value(1).toString()));
        ui->uaer_data_show->setItem(row_cout,2,new QTableWidgetItem(query.value(2).toString()));
        ui->uaer_data_show->setItem(row_cout,3,new QTableWidgetItem(query.value(3).toString()));
        ui->uaer_data_show->setItem(row_cout,4,new QTableWidgetItem(query.value(4).toString()));
        ui->uaer_data_show->setItem(row_cout,5,new QTableWidgetItem(query.value(5).toString()));
        ui->uaer_data_show->setItem(row_cout,6,new QTableWidgetItem(query.value(6).toString()));
        break;
    }
    if(ui->uaer_data_show->rowCount()>0)     //判断是否有数据存在
    {
        ui->action->setEnabled(true);
        ui->action_delete->setEnabled(true);
        ui->action_out->setEnabled(true);
    }
    if(ui->uaer_data_show->rowCount()>1)     //判断是否有数据存在
    {
        ui->action_7->setEnabled(true);
        ui->action_8->setEnabled(true);
        ui->action_9->setEnabled(true);
        ui->action_10->setEnabled(true);
    }
    dbopen.closedb();
    ui->uaer_data_show->show();
    settablewidgetcolor();
    makeday();
    //chushihua();
    int max =ui->uaer_data_show->verticalScrollBar()->maximum();
    ui->uaer_data_show->verticalScrollBar()->setMaximum(max+1);
    ui->uaer_data_show->verticalScrollBar()->setValue(max+1);
}
//关闭
void MainWindow::on_action_2_triggered()
{
    this->close();
}
//取消自动登录
void MainWindow::on_action_3_triggered()
{
    canclelogin();
}
//取消自动登录
void MainWindow::canclelogin()
{
    userdata = dbopen.initDatabase("User_recode.db");
    QSqlQuery query(userdata);
    QSqlQuery query2(userdata);
    query.exec("select * from User_login");
    query2.exec("select * from User_recode");
    while (query.next())
    {
        query2.last();
        QString sql_updata ="update User_login set User_logintip=";
        sql_updata +="'false' where User_name= ";
        sql_updata +="'";
        sql_updata += query2.value(1).toString();
        sql_updata +="'";
        query.exec(sql_updata);
        dbopen.closedb();
        break;
    }
}
//时间排序槽函数
void MainWindow::on_action_7_triggered()
{
    Sequence_time();
}
//类型排序槽函数
void MainWindow::on_action_8_triggered()
{
    Sequence_type();
}
//金额升序排序槽函数
void MainWindow::on_action_9_triggered()
{
    moneyupto();
}
//金额降序排序槽函数
void MainWindow::on_action_10_triggered()
{
    moneydownto();
}
//获取当前坐标行//更新数据库系统
void MainWindow::on_uaer_data_show_cellClicked(int row, int column)
{
    the_row_number = row;
    if(the_row!=the_row_number&&column!=-1)
    {
        ui->uaer_data_show->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->uaer_data_show->closePersistentEditor(item1);
        ui->uaer_data_show->closePersistentEditor(item2);
        ui->uaer_data_show->closePersistentEditor(item3);
        ui->uaer_data_show->closePersistentEditor(item4);
        ui->uaer_data_show->closePersistentEditor(item5);
        ui->uaer_data_show->closePersistentEditor(item6);
        if(item1!=NULL)
        {
            userdata = dbopen.initDatabase(userdataname);
            QSqlQuery query(userdata);
            query.exec("select * from UserData");
            QString user_data_number,data_time,data_type,data_services,data_money,data_pay_type,data_remark;
            user_data_number = item0->text();
            data_time = item1->text();
            data_type = item2->text();
            data_services = item3->text();
            data_money = item4->text();
            data_pay_type = item5->text();
            data_remark = item6->text();
            while (query.next())
            {
                if(query.value(0)==user_data_number)
                {
                    QString sql = "update UserData set Data_time=";
                    sql +="'";
                    sql += data_time;
                    sql +="',";
                    sql += "Data_type=";
                    sql += "'";
                    sql += data_type;
                    sql += "',";
                    sql += "Data_services=";
                    sql += "'";
                    sql += data_services;
                    sql += "',";
                    sql += "Data_money=";
                    sql += "'";
                    sql += data_money;
                    sql += "',";
                    sql += "Data_pay_type=";
                    sql += "'";
                    sql += data_pay_type;
                    sql += "',";
                    sql += "Data_ramark=";
                    sql += "'";
                    sql += data_remark;
                    sql += "'";
                    sql += " where User_data_number=";
                    sql += "'";
                    sql += user_data_number;
                    sql += "'";
                    query.exec(sql);
                }
            }
            dbopen.closedb();
            item0 = NULL;
            item1 = NULL;
            item2 = NULL;
            item3 = NULL;
            item4 = NULL;
            item5 = NULL;
            item6 = NULL;
            combox = NULL;//地址释放，注意每一个对象的生命周期
        }
        settablewidgetcolor();
        makeday();
    }
}
//创建菜单
void MainWindow::createActions()
{
    pop_menu = new QMenu();
    action_add = new QAction(this);   //添加
    action_revise = new QAction(this);  //修改
    action_delete = new QAction(this);  //删除
    action_Sequence = new QAction(this);  //排序
    action_time = new QAction(this);     //时间排序
    action_type = new QAction(this);   //收支类型排序
    moneyup = new QAction(this);//金钱増序
    moneydown = new QAction(this);//金钱降序
    //设置每一个action名称
    action_add->setText("添加");
    action_revise->setText("修改");
    action_delete->setText("删除");
    action_Sequence->setText("排序");
    action_time->setText("时间排序");
    action_type->setText("收支类型排序");
    moneyup->setText("金额増序");
    moneydown->setText("金额降序");
    //建立每个右键想对应的槽函数
    connect(action_add,SIGNAL(triggered(bool)),this,SLOT(adduserdata()));  //添加
    connect(action_revise,SIGNAL(triggered(bool)),this,SLOT(reviseuserdata()));  //修改
    connect(action_delete,SIGNAL(triggered(bool)),this,SLOT(deleteuserdata()));   //删除
    connect(action_time,SIGNAL(triggered(bool)),this,SLOT(Sequence_time()));   //时间排序
    connect(action_type,SIGNAL(triggered(bool)),this,SLOT(Sequence_type()));    //支付类型排序
    connect(moneyup,SIGNAL(triggered(bool)),this,SLOT(moneyupto())); //増序排列
    connect(moneydown,SIGNAL(triggered(bool)),this,SLOT(moneydownto()));//降序排列
}
//鼠标右键添加
void MainWindow::adduserdata()
{
    on_action_1_triggered();
}
//鼠标右键修改
void MainWindow::reviseuserdata()
{
    the_row = ui->uaer_data_show->currentRow();
    item0 = ui->uaer_data_show->item(the_row,0);
    item1 = ui->uaer_data_show->item(the_row,1);
    item2 = ui->uaer_data_show->item(the_row,2);
    item3 = ui->uaer_data_show->item(the_row,3);
    item4 = ui->uaer_data_show->item(the_row,4);
    item5 = ui->uaer_data_show->item(the_row,5);
    item6 = ui->uaer_data_show->item(the_row,6);

    ui->uaer_data_show->setCurrentCell(the_row,1);  //设定光标

    ui->uaer_data_show->openPersistentEditor(item1);  //开编辑
    ui->uaer_data_show->openPersistentEditor(item2);
    ui->uaer_data_show->openPersistentEditor(item3);
    ui->uaer_data_show->openPersistentEditor(item4);
    ui->uaer_data_show->openPersistentEditor(item5);
    ui->uaer_data_show->openPersistentEditor(item6);

    ui->uaer_data_show->editItem(item1);
    ui->uaer_data_show->editItem(item2);
    ui->uaer_data_show->editItem(item3);
    ui->uaer_data_show->editItem(item4);
    ui->uaer_data_show->editItem(item5);
    ui->uaer_data_show->editItem(item6);

    QString userdata1 = item3->text();
    QString userdata2 = item5->text();
    if(item2->text() == "存入")
    {
        combox = new QComboBox(this);
        combox1 = new QComboBox(this);
        combox2 = new QComboBox(this);
        QStringList data1,data2;
        data1<<userdata2<<"现金"<<"优惠卷"<<"支票"<<"银行转账";
        data2<<userdata1<<"公司工资"<<"公司奖金"<<"优惠券"<<"彩票"<<"别人赠送";
        combox->addItem("存入");
        combox->addItem("支出");
        combox1->addItems(data1);
        combox2->addItems(data2);
        ui->uaer_data_show->setCellWidget(the_row,2,(QWidget*)combox);
        ui->uaer_data_show->setCellWidget(the_row,3,(QWidget*)combox2);
        ui->uaer_data_show->setCellWidget(the_row,5,(QWidget*)combox1);
        connect(combox,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox1,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox2,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox,SIGNAL(currentTextChanged(QString)),this,SLOT(changemode()));
        combox1 ->setEditable(true);
        combox2 ->setEditable(true);
    }
    else if(item2->text() == "支出")
    {
        combox = new QComboBox(this);
        combox1 = new QComboBox(this);
        combox2 = new QComboBox(this);
        QStringList data1,data2;
        data1<<userdata2<<"现金"<<"优惠卷"<<"支票"<<"银行卡"<<"信用卡";
        data2<<userdata1<<"吃饭"<<"教育"<<"购物"<<"送礼"<<"生活"<<"保险";
        combox->addItem("支出");
        combox->addItem("存入");
        combox1->addItems(data1);
        combox2->addItems(data2);
        ui->uaer_data_show->setCellWidget(the_row,2,(QWidget*)combox);
        ui->uaer_data_show->setCellWidget(the_row,3,(QWidget*)combox2);
        ui->uaer_data_show->setCellWidget(the_row,5,(QWidget*)combox1);
        connect(combox,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox1,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox2,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox,SIGNAL(currentTextChanged(QString)),this,SLOT(changemode()));
        combox1 ->setEditable(true);
        combox2 ->setEditable(true);
    }
    else
    {
        combox = new QComboBox(this);
        combox1 = new QComboBox(this);
        combox2 = new QComboBox(this);
        QStringList data1,data2;
        data1<<userdata2<<"现金"<<"优惠卷"<<"支票"<<"银行转账";
        data2<<userdata1<<"公司工资"<<"公司奖金"<<"优惠券"<<"彩票"<<"别人赠送";
        combox->addItem("存入");
        combox->addItem("支出");
        combox1->addItems(data1);
        combox2->addItems(data2);
        ui->uaer_data_show->setCellWidget(the_row,2,(QWidget*)combox);
        ui->uaer_data_show->setCellWidget(the_row,3,(QWidget*)combox2);
        ui->uaer_data_show->setCellWidget(the_row,5,(QWidget*)combox1);
        connect(combox,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox1,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox2,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox,SIGNAL(currentTextChanged(QString)),this,SLOT(changemode()));
        combox1 ->setEditable(true);
        combox2 ->setEditable(true);
    }
}
//改变模式，通过监听收支类型的combox，从而改变事物和支付方式
void MainWindow::changemode()
{
    if(combox->currentText() == "存入")
    {
        delete combox1;
        delete combox2;
        combox1 = NULL;
        combox2 = NULL;
        combox1 = new QComboBox(this);
        combox2 = new QComboBox(this);
        QStringList data1,data2;
        data1<<"现金"<<"优惠卷"<<"支票"<<"银行转账";
        data2<<"公司工资"<<"公司奖金"<<"优惠券"<<"彩票"<<"别人赠送";
        combox1->addItems(data1);
        combox2->addItems(data2);
        ui->uaer_data_show->setCellWidget(the_row,3,(QWidget*)combox2);
        ui->uaer_data_show->setCellWidget(the_row,5,(QWidget*)combox1);
        connect(combox1,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox2,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        combox1 ->setEditable(true);
        combox2 ->setEditable(true);
    }
    else if(combox->currentText() == "支出")
    {
        delete combox1;
        delete combox2;
        combox1 = NULL;
        combox2 = NULL;
        combox1 = new QComboBox(this);
        combox2 = new QComboBox(this);
        QStringList data1,data2;
        data1<<"现金"<<"优惠卷"<<"支票"<<"银行卡"<<"信用卡";
        data2<<"吃饭"<<"教育"<<"购物"<<"送礼"<<"生活"<<"保险";
        combox1->addItems(data1);
        combox2->addItems(data2);
        ui->uaer_data_show->setCellWidget(the_row,3,(QWidget*)combox2);
        ui->uaer_data_show->setCellWidget(the_row,5,(QWidget*)combox1);
        connect(combox1,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        connect(combox2,SIGNAL(currentTextChanged(QString)),this,SLOT(setcombox()));
        combox1 ->setEditable(true);
        combox2 ->setEditable(true);
    }
}
//设置槽函数
void MainWindow::setcombox()
{
    item2->setText(combox->currentText());
    item3->setText(combox2->currentText());
    item5->setText(combox1->currentText());
//    combox1 = NULL;
//    combox2 = NULL;
}
//鼠标右键删除
void MainWindow::deleteuserdata()
{
    int flag =QMessageBox::information(this,"提示","删除操作为不可逆，请慎重",QMessageBox::Yes|QMessageBox::No);
    switch (flag) {
        break;
    case QMessageBox::Yes:
        deleteuserdata_t();
        break;
    case QMessageBox::No:
    default:
        break;
    }
    if((ui->uaer_data_show->rowCount())==0)      //判断是否有数据存在，如果没有，则将部分按钮设置为不可用
    {
        ui->action->setEnabled(false);
        ui->action_delete->setEnabled(false);
        ui->action_out->setEnabled(false);
        ui->action_7->setEnabled(false);
        ui->action_8->setEnabled(false);
        ui->action_9->setEnabled(false);
        ui->action_10->setEnabled(false);
    }
    if((ui->uaer_data_show->rowCount())<2)      //判断是否有数据存在，如果没有，则将部分按钮设置为不可用
    {
        ui->action_7->setEnabled(false);
        ui->action_8->setEnabled(false);
        ui->action_9->setEnabled(false);
        ui->action_10->setEnabled(false);
    }
}
//删除
void MainWindow::deleteuserdata_t()
{
    int the_row = ui->uaer_data_show->currentRow();
    QString usernumber =  ui->uaer_data_show->item(the_row,0)->text();
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    query.exec("select * from UserData");
    while (query.next())
    {  if(query.value(0).toString()==usernumber)
        {
            QString sql = "delete from UserData where User_data_number=";
            sql+="'";
            sql += usernumber;
            sql+="'";
            query.exec(sql);
            dbopen.closedb();
            break;
        }
    }
    ui->uaer_data_show->removeRow(the_row);
    ui->uaer_data_show->show();
    settablewidgetcolor();
    makeday();
    int max =ui->uaer_data_show->verticalScrollBar()->maximum();
    ui->uaer_data_show->verticalScrollBar()->setMaximum(max+1);
    ui->uaer_data_show->verticalScrollBar()->setValue(max+1);
}

//鼠标右键时间排序
void MainWindow::Sequence_time()
{
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    query.exec("select * from UserData order by Data_time DESC");
    while (ui->uaer_data_show->rowCount())
    {
        ui->uaer_data_show->removeRow((ui->uaer_data_show->rowCount()-1));
    }
    while (query.next())
    {
        int row_cout = ui->uaer_data_show->rowCount();
        ui->uaer_data_show->insertRow(row_cout);
        ui->uaer_data_show->setItem(row_cout,0,new QTableWidgetItem(query.value(0).toString()));
        ui->uaer_data_show->setItem(row_cout,1,new QTableWidgetItem(query.value(1).toString()));
        ui->uaer_data_show->setItem(row_cout,2,new QTableWidgetItem(query.value(2).toString()));
        ui->uaer_data_show->setItem(row_cout,3,new QTableWidgetItem(query.value(3).toString()));
        ui->uaer_data_show->setItem(row_cout,4,new QTableWidgetItem(query.value(4).toString()));
        ui->uaer_data_show->setItem(row_cout,5,new QTableWidgetItem(query.value(5).toString()));
        ui->uaer_data_show->setItem(row_cout,6,new QTableWidgetItem(query.value(6).toString()));
    }
    ui->uaer_data_show->show();
    dbopen.closedb();
    settablewidgetcolor();
}
//金钱増序排列
void MainWindow::moneyupto()
{
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    query.exec("select * from UserData order by Data_money ASC");
    while (ui->uaer_data_show->rowCount()) {
        ui->uaer_data_show->removeRow((ui->uaer_data_show->rowCount()-1));
    }
    while (query.next())
    {
        int row_cout = ui->uaer_data_show->rowCount();
        ui->uaer_data_show->insertRow(row_cout);
        ui->uaer_data_show->setItem(row_cout,0,new QTableWidgetItem(query.value(0).toString()));
        ui->uaer_data_show->setItem(row_cout,1,new QTableWidgetItem(query.value(1).toString()));
        ui->uaer_data_show->setItem(row_cout,2,new QTableWidgetItem(query.value(2).toString()));
        ui->uaer_data_show->setItem(row_cout,3,new QTableWidgetItem(query.value(3).toString()));
        ui->uaer_data_show->setItem(row_cout,4,new QTableWidgetItem(query.value(4).toString()));
        ui->uaer_data_show->setItem(row_cout,5,new QTableWidgetItem(query.value(5).toString()));
        ui->uaer_data_show->setItem(row_cout,6,new QTableWidgetItem(query.value(6).toString()));
    }
    ui->uaer_data_show->show();
    dbopen.closedb();
    settablewidgetcolor();
}
//金钱降序排列
void MainWindow::moneydownto()
{
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    query.exec("select * from UserData order by Data_money DESC");
    while (ui->uaer_data_show->rowCount()) {
        ui->uaer_data_show->removeRow((ui->uaer_data_show->rowCount()-1));
    }
    while (query.next())
    {
        int row_cout = ui->uaer_data_show->rowCount();
        ui->uaer_data_show->insertRow(row_cout);
        ui->uaer_data_show->setItem(row_cout,0,new QTableWidgetItem(query.value(0).toString()));
        ui->uaer_data_show->setItem(row_cout,1,new QTableWidgetItem(query.value(1).toString()));
        ui->uaer_data_show->setItem(row_cout,2,new QTableWidgetItem(query.value(2).toString()));
        ui->uaer_data_show->setItem(row_cout,3,new QTableWidgetItem(query.value(3).toString()));
        ui->uaer_data_show->setItem(row_cout,4,new QTableWidgetItem(query.value(4).toString()));
        ui->uaer_data_show->setItem(row_cout,5,new QTableWidgetItem(query.value(5).toString()));
        ui->uaer_data_show->setItem(row_cout,6,new QTableWidgetItem(query.value(6).toString()));
    }
    ui->uaer_data_show->show();
    dbopen.closedb();
    settablewidgetcolor();
}
//按照支付类型排序
void MainWindow::Sequence_type()
{
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    query.exec("select * from UserData order by Data_type ASC");
    while (ui->uaer_data_show->rowCount()) {
        ui->uaer_data_show->removeRow((ui->uaer_data_show->rowCount()-1));
    }
    while (query.next())
    {
        int row_cout = ui->uaer_data_show->rowCount();
        ui->uaer_data_show->insertRow(row_cout);
        ui->uaer_data_show->setItem(row_cout,0,new QTableWidgetItem(query.value(0).toString()));
        ui->uaer_data_show->setItem(row_cout,1,new QTableWidgetItem(query.value(1).toString()));
        ui->uaer_data_show->setItem(row_cout,2,new QTableWidgetItem(query.value(2).toString()));
        ui->uaer_data_show->setItem(row_cout,3,new QTableWidgetItem(query.value(3).toString()));
        ui->uaer_data_show->setItem(row_cout,4,new QTableWidgetItem(query.value(4).toString()));
        ui->uaer_data_show->setItem(row_cout,5,new QTableWidgetItem(query.value(5).toString()));
        ui->uaer_data_show->setItem(row_cout,6,new QTableWidgetItem(query.value(6).toString()));
    }
    ui->uaer_data_show->show();
    dbopen.closedb();
    settablewidgetcolor();
}

//重载鼠标点击事件
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    pop_menu->clear();      //模仿写的
    QPoint point = event->pos();
    QTableWidgetItem *item = ui->uaer_data_show->itemAt(point);
    if(item != NULL)
    {
        pop_menu->addAction(action_add);
        pop_menu->addAction(action_revise);
        pop_menu->addAction(action_delete);
        pop_menu->addSeparator();
        sort_style = pop_menu->addMenu("排序");
        sort_style->addAction(action_time);
        sort_style->addAction(action_type);
        sort_style->addAction(moneyup);
        sort_style->addAction(moneydown);
        //菜单出现的位置为当前鼠标的位置
        pop_menu->exec(QCursor::pos());
        event->accept();
     }
}
//修改界面
void MainWindow::on_action_triggered()
{
    reviseuserdata();
}
//导出数据
void MainWindow::on_action_out_triggered()
{
    ExcelEngine excle(QObject::tr("D:\\MR.Butler.xls"));
    excle.Open();
    excle.SaveDataFrTable(ui->uaer_data_show);
    excle.Close();
   QMessageBox::information(NULL, "成功", "导出成功！请在D盘根目录查看",QMessageBox::Yes);
}
//调用系统计算器
void MainWindow::on_action_5_triggered()
{
    QProcess::startDetached("C:/Windows/System32/calc.exe");
}
//备忘录    /* * /* / */* /*/*/* /
void MainWindow::on_action_4_triggered()
{
    QProcess::startDetached("C:/Windows/System32/notepad.exe");
}
//删除界面
void MainWindow::on_action_delete_triggered()   //槽函数
{
    deleteuserdata();
}
//绘制图形函数
void MainWindow::paintEvent(QPaintEvent *)
{
    int x = ui->widget->geometry().x();
    int y = ui->widget->geometry().y()+50;
    painterout->begin(this);
    if(Min>1.0)
    {
        Min = 1.0;
    }
    painterout->setBrush(QBrush(QColor(255,0,0,200),Qt::SolidPattern));
    painterout->drawPie(x+50,y+30,150,150,(90*16),(Min*360*16));
    painterout->end();
    painterin->begin(this);
    painterin->setBrush(QBrush(Qt::blue,Qt::SolidPattern));
    painterin->drawPie(x+50,y+30,150,150,(90*16+Min*360*16),((1-Min)*360*16));
    painterin->end();
    painter1->begin(this);
    //下面的主要是为了绘制两个图例
    painter1->setBrush(QBrush(QColor(255,0,0,200),Qt::SolidPattern));
    painter1->drawRect(x+70,y+220,10,10);
    painter1->end();
    painter2->begin(this);
    painter2->setBrush(QBrush(Qt::blue,Qt::SolidPattern));
    painter2->drawRect(x+70,y+235,10,10);
    painter2->end();
    dbopen.closedb();
}
//绘制用户当天的消费统计图
void MainWindow::makeday()
{
    double in = 0,out= 0;
    QDateTime dt;
    QDate date;
    dt.setDate(date.currentDate());
    QString data = dt.toString("yyyy-MM-dd");
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    QString sql ="select * from UserData where Data_time like'";
    sql += data;
    sql += "%'";
    query.exec(sql);
    while (query.next())
    {
        if((query.value(2).toString())=="存入")
        {
            in += query.value(4).toString().toDouble();
        }
        if(query.value(2).toString() == "支出")
        {
            out += query.value(4).toString().toDouble();
        }
    }
    if(in == 0)
    {
        Min =1;
    }
    else
    {
        Min = out/in;
    }
    ui->shouruedit->setNum(in);
    ui->zhichuedit->setNum(out);
    this->update();
    dbopen.closedb();
    timer->stop();
}

//当月统计图
void MainWindow::on_theday_clicked()
{
    double in = 0,out= 0;
    QDateTime dt;
    QDate date;
    dt.setDate(date.currentDate());
    QString data = dt.toString("yyyy-MM");
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    QString sql ="select * from UserData where Data_time like'";
    sql += data;
    sql += "%'";
    query.exec(sql);
    while (query.next())
    {
        if((query.value(2).toString())=="存入")
        {
            in += query.value(4).toString().toDouble();
        }
        if(query.value(2).toString() == "支出")
        {
            out += query.value(4).toString().toDouble();
        }
    }
    if(in == 0)
    {
        Min =1;
    }
    else
    {
        Min = out/in;
    }
    ui->shouruedit->setNum(in);
    ui->zhichuedit->setNum(out);
    this->update();
    dbopen.closedb();
    timer->start();
}
//当年统计图
void MainWindow::on_themon_clicked()
{
    double in = 0,out= 0;
    QDateTime dt;
    QDate date;
    dt.setDate(date.currentDate());
    QString data = dt.toString("yyyy");
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    QString sql ="select * from UserData where Data_time like'";
    sql += data;
    sql += "%'";
    query.exec(sql);
    while (query.next())
    {
        if((query.value(2).toString())=="存入")
        {
            in += query.value(4).toString().toDouble();
        }
        if(query.value(2).toString() == "支出")
        {
            out += query.value(4).toString().toDouble();
        }
    }
    if(in == 0)
    {
        Min =1;
    }
    else
    {
        Min = out/in;
    }
    ui->shouruedit->setNum(in);
    ui->zhichuedit->setNum(out);
    this->update();
    timer->start();
}
//全部统计图
void MainWindow::on_theyear_clicked()
{
    double in = 0,out= 0;
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    QString sql ="select * from UserData";
    query.exec(sql);
    while (query.next())
    {
        if((query.value(2).toString())=="存入")
        {
            in += query.value(4).toString().toDouble();
        }
        if(query.value(2).toString() == "支出")
        {
            out += query.value(4).toString().toDouble();
        }
    }
    if(in == 0)
    {
        Min =1;
    }
    else
    {
        Min = out/in;
    }
    ui->shouruedit->setNum(in);
    ui->zhichuedit->setNum(out);
    this->update();
    timer->start();
}
//修改颜色的
void MainWindow::settablewidgetcolor()
{
    if(colortip)
    {
        int curent_row = ui->uaer_data_show->rowCount();
        for(int i=0;i<curent_row;i++)
        {
            item0 = ui->uaer_data_show->item(i,0);
            item1 = ui->uaer_data_show->item(i,1);
            item2 = ui->uaer_data_show->item(i,2);
            item3 = ui->uaer_data_show->item(i,3);
            item4 = ui->uaer_data_show->item(i,4);
            item5 = ui->uaer_data_show->item(i,5);
            item6 = ui->uaer_data_show->item(i,6);
            if(item2->text()=="支出")
            {
                item0->setBackgroundColor(QColor(255,0,0,200));
                item1->setBackgroundColor(QColor(255,0,0,200));
                item2->setBackgroundColor(QColor(255,0,0,200));
                item3->setBackgroundColor(QColor(255,0,0,200));
                item4->setBackgroundColor(QColor(255,0,0,200));
                item5->setBackgroundColor(QColor(255,0,0,200));
                item6->setBackgroundColor(QColor(255,0,0,200));
            }
            if(item2->text()=="存入")
            {
                item0->setBackgroundColor(QColor(255,255,255,0));
                item1->setBackgroundColor(QColor(255,255,255,0));
                item2->setBackgroundColor(QColor(255,255,255,0));
                item3->setBackgroundColor(QColor(255,255,255,0));
                item4->setBackgroundColor(QColor(255,255,255,0));
                item5->setBackgroundColor(QColor(255,255,255,0));
                item6->setBackgroundColor(QColor(255,255,255,0));
            }
            item0 = NULL;
            item1 = NULL;
            item2 = NULL;
            item3 = NULL;
            item4 = NULL;
            item5 = NULL;
            item6 = NULL;
        }
    }
    else
    {
        int curent_row = ui->uaer_data_show->rowCount();
        for(int i=0;i<curent_row;i++)
        {
            item0 = ui->uaer_data_show->item(i,0);
            item1 = ui->uaer_data_show->item(i,1);
            item2 = ui->uaer_data_show->item(i,2);
            item3 = ui->uaer_data_show->item(i,3);
            item4 = ui->uaer_data_show->item(i,4);
            item5 = ui->uaer_data_show->item(i,5);
            item6 = ui->uaer_data_show->item(i,6);
            if(item2->text()=="支出")
            {
                item0->setBackgroundColor(QColor(169,169,169,200));
                item1->setBackgroundColor(QColor(169,169,169,200));
                item2->setBackgroundColor(QColor(169,169,169,200));
                item3->setBackgroundColor(QColor(169,169,169,200));
                item4->setBackgroundColor(QColor(169,169,169,200));
                item5->setBackgroundColor(QColor(169,169,169,200));
                item6->setBackgroundColor(QColor(169,169,169,200));
            }
            if(item2->text()=="存入")
            {
                item0->setBackgroundColor(QColor(255,255,255,0));
                item1->setBackgroundColor(QColor(255,255,255,0));
                item2->setBackgroundColor(QColor(255,255,255,0));
                item3->setBackgroundColor(QColor(255,255,255,0));
                item4->setBackgroundColor(QColor(255,255,255,0));
                item5->setBackgroundColor(QColor(255,255,255,0));
                item6->setBackgroundColor(QColor(255,255,255,0));
            }
            item0 = NULL;
            item1 = NULL;
            item2 = NULL;
            item3 = NULL;
            item4 = NULL;
            item5 = NULL;
            item6 = NULL;
        }
    }
}
//取消红色标记   //为人民服务
void MainWindow::on_cancalred_triggered()
{
    colortip = !colortip;
    settablewidgetcolor();
    if(!colortip)
    {
        ui->cancalred->setText("设置红色标记");
        ui->cancalred->setIcon(QIcon(":/mainwindows/images/mainwindows/star.png"));
    }
    else
    {
        ui->cancalred->setText("取消红色标记");
        ui->cancalred->setIcon(QIcon(":/mainwindows/images/mainwindows/star_3.png"));
    }
}
//查询记录时间转化为字符串
void MainWindow::on_time1_editingFinished()
{
    userdatatime1 =  ui->time1->dateTime().toString("yyyy-MM-dd");
}

void MainWindow::on_time2_editingFinished()
{
    userdatatime2 =  ui->time2->dateTime().toString("yyyy-MM-dd");
}
//查询按钮
void MainWindow::on_select_clicked()
{
    userdatatime2 =  ui->time2->dateTime().toString("yyyy-MM-dd");
    userdatatime1 =  ui->time1->dateTime().toString("yyyy-MM-dd");
    if(userdatatime1.mid(0,4).toInt()>userdatatime2.mid(0,4).toInt())   //解决用户手贱问题
    {
        QString temp = userdatatime1;
        userdatatime1 = userdatatime2;
        userdatatime2 = temp;
    }
    else if((userdatatime1.mid(5,2).toInt()>userdatatime2.mid(5,2).toInt())&&(userdatatime1.mid(0,4).toInt()>=userdatatime2.mid(0,4).toInt()))
    {
        QString temp = userdatatime1;
        userdatatime1 = userdatatime2;
        userdatatime2 = temp;
    }
    else if((userdatatime1.mid(8,2).toInt()>userdatatime2.mid(8,2).toInt())&&((userdatatime1.mid(5,2).toInt()>userdatatime2.mid(5,2).toInt())||(userdatatime1.mid(0,4).toInt()>=userdatatime2.mid(0,4).toInt())))
    {
        QString temp = userdatatime1;
        userdatatime1 = userdatatime2;
        userdatatime2 = temp;
    }
    ui->cancle->setEnabled(true);
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query(userdata);
    QString sql;
    int money1 = ui->moneyedit1->text().toInt();
    int money2 = ui->moneyedit2->text().toInt();
    if(money1>money2)    //解决用后手贱问题
    {
        int x= money1;
        money1=money2;
        money2=x;
    }
    QString mon1 = QString::number(money1,10);
    QString mon2 = QString::number(money2,10);
    double allshouru=0.0,allzhichu=0.0;       
    if((money1==0)&&(money2==0)&&(ui->radioButton3->isChecked()))
    {
        sql = "select * from UserData where Data_time BETWEEN '";
        sql += userdatatime1;
        sql += "' AND '";
        sql += userdatatime2;
        sql += "'";
    }
    else if((money1==0)&&(money2==0)&&(ui->radioButton1->isChecked()))
    {
        sql = "select * from UserData where Data_time BETWEEN '";
        sql += userdatatime1;
        sql += "' AND '";
        sql += userdatatime2;
        sql += "' AND Data_type = '";
        sql += "存入'";
    }
    else if((money1==0)&&(money2==0)&&(ui->radioButton2->isChecked()))
    {
        sql = "select * from UserData where Data_time BETWEEN '";
        sql += userdatatime1;
        sql += "' AND '";
        sql += userdatatime2;
        sql += "' AND Data_type = '";
        sql += "支出'";
    }
    else if(((money1!=0)||(money2!=0))&&(ui->radioButton3->isChecked()))
    {
        sql = "select * from UserData where Data_time BETWEEN '";
        sql += userdatatime1;
        sql += "' AND '";
        sql += userdatatime2;
        sql += "' AND ";
        sql += "Data_money BETWEEN '";
        sql += mon1;
        sql += "' AND '";
        sql += mon2;
        sql +="'";
    }
    else if(((money1!=0)||(money2!=0))&&(ui->radioButton1->isChecked()))
    {
        sql = "select * from UserData where Data_time BETWEEN '";
        sql += userdatatime1;
        sql += "' AND '";
        sql += userdatatime2;
        sql += "' AND ";
        sql += "Data_money BETWEEN '";
        sql += mon1;
        sql += "' AND '";
        sql += mon2;
        sql += "' AND Data_type = '";
        sql += "存入'";
    }
    else if(((money1!=0)||(money2!=0))&&(ui->radioButton2->isChecked()))
    {
        sql = "select * from UserData where Data_time BETWEEN '";
        sql += userdatatime1;
        sql += "' AND '";
        sql += userdatatime2;
        sql += "' AND ";
        sql += "Data_money BETWEEN '";
        sql += mon1;
        sql += "' AND '";
        sql += mon2;
        sql += "' AND Data_type = '";
        sql += "支出'";
    }
    qDebug()<<sql;
    query.exec(sql);
    ui->uaer_data_show->clear();
    //这个方式，最终会拖慢这个程序的运行速度的   //暂时还没有找到其他的更好的解决方法20150203
    while (ui->uaer_data_show->rowCount())
    {
        ui->uaer_data_show->removeRow((ui->uaer_data_show->rowCount()-1));
    }
    maketable();    //这个是不能忘记的
    while (query.next())
       {
           int row_cout = ui->uaer_data_show->rowCount();
           ui->uaer_data_show->insertRow(row_cout);
           ui->uaer_data_show->setItem(row_cout,0,new QTableWidgetItem(query.value(0).toString()));
           ui->uaer_data_show->setItem(row_cout,1,new QTableWidgetItem(query.value(1).toString()));
           ui->uaer_data_show->setItem(row_cout,2,new QTableWidgetItem(query.value(2).toString()));
           ui->uaer_data_show->setItem(row_cout,3,new QTableWidgetItem(query.value(3).toString()));
           ui->uaer_data_show->setItem(row_cout,4,new QTableWidgetItem(query.value(4).toString()));
           ui->uaer_data_show->setItem(row_cout,5,new QTableWidgetItem(query.value(5).toString()));
           ui->uaer_data_show->setItem(row_cout,6,new QTableWidgetItem(query.value(6).toString()));
           if(query.value(2).toString() == "存入")
           {
               allshouru += query.value(4).toString().toDouble();
           }
           if(query.value(2).toString() == "支出")
           {
               allzhichu += query.value(4).toString().toDouble();
           }
           row_cout ++;
        }

       ui->uaer_data_show->show();
       dbopen.closedb();
       settablewidgetcolor();
       //修饰字体
       QFont font1;
       font1.setBold(true);
       font1.setPointSize(12);
       ui->allmonetin->setFont(font1);
       ui->allmoneyout->setFont(font1);
       //字体复制
       ui->allmonetin->setNum(allshouru);
       ui->allmoneyout->setNum(allzhichu);
       ui->shouruedit->setNum(allshouru);
       ui->zhichuedit->setNum(allzhichu);
       //饼状图初始化
       Min = allzhichu/allshouru;
       this->update();
        //禁用功能键
       ui->action->setEnabled(false);
       ui->action_1->setEnabled(false);
       ui->action_7->setEnabled(false);
       ui->action_8->setEnabled(false);
       ui->action_9->setEnabled(false);
       ui->action_10->setEnabled(false);
       ui->action_delete->setEnabled(false);
       action_add->setEnabled(false);
       action_revise->setEnabled(false);
       action_delete->setEnabled(false);
       action_Sequence->setEnabled(false);
       action_time->setEnabled(false);
       action_type->setEnabled(false);
       moneyup->setEnabled(false);
       moneydown->setEnabled(false);
}
//返回按钮
void MainWindow::on_cancle_clicked()
{
    //返回初始化数据
    ui->cancle->setEnabled(false);
    insertdata();
    maketable();
    //启用功能键
    ui->action->setEnabled(true);
    ui->action_1->setEnabled(true);
    ui->action_7->setEnabled(true);
    ui->action_8->setEnabled(true);
    ui->action_9->setEnabled(true);
    ui->action_10->setEnabled(true);
    ui->action_delete->setEnabled(true);
    action_add->setEnabled(true);
    action_revise->setEnabled(true);
    action_delete->setEnabled(true);
    action_Sequence->setEnabled(true);
    action_time->setEnabled(true);
    action_type->setEnabled(true);
    moneyup->setEnabled(true);
    moneydown->setEnabled(true);

    //恢复到初始化
    ui->moneyedit1->setText("0.0");
    ui->moneyedit2->setText("0.0");
    ui->radioButton3->setChecked(true);
    ui->allmonetin->setText("0.0");
    ui->allmoneyout->setText("0.0");
    ui->shouruedit->setText("0.0");
    ui->zhichuedit->setText("0.0");
}

void MainWindow::on_changepassword_triggered()
{
//    Add_user_data useradd;
//    if(useradd.exec()==QDialog::Accepted)   //界面交互
//    {
//        addusershow();
//        this->update();
//    }

    Changeuserinfor changeuserinfor;
    changeuserinfor.userchangeinforname = userchangeinforname;
    changeuserinfor.timer.start(10);
    if(changeuserinfor.exec() == QDialog::Accepted)
    {
        this->update();
    }
}
//将数据倒叙输出，解决多数据显示问题  目前还没有找到横好的方法     问题与20150114解决，此函数作废
//void MainWindow::chushihua()
//{
//    userdata = dbopen.initDatabase(userdataname);
//    QSqlQuery query(userdata);
//    query.exec("select * from UserData order by User_data_number DESC");
//    while (ui->uaer_data_show->rowCount()) {
//        ui->uaer_data_show->removeRow((ui->uaer_data_show->rowCount()-1));
//    }
//    while (query.next())
//    {
//        int row_cout = ui->uaer_data_show->rowCount();
//        ui->uaer_data_show->insertRow(row_cout);
//        ui->uaer_data_show->setItem(row_cout,0,new QTableWidgetItem(query.value(0).toString()));
//        ui->uaer_data_show->setItem(row_cout,1,new QTableWidgetItem(query.value(1).toString()));
//        ui->uaer_data_show->setItem(row_cout,2,new QTableWidgetItem(query.value(2).toString()));
//        ui->uaer_data_show->setItem(row_cout,3,new QTableWidgetItem(query.value(3).toString()));
//        ui->uaer_data_show->setItem(row_cout,4,new QTableWidgetItem(query.value(4).toString()));
//        ui->uaer_data_show->setItem(row_cout,5,new QTableWidgetItem(query.value(5).toString()));
//        ui->uaer_data_show->setItem(row_cout,6,new QTableWidgetItem(query.value(6).toString()));
//    }
//    ui->uaer_data_show->show();
//    dbopen.closedb();
//    settablewidgetcolor();
//    select * from UserData where Data_time BETWEEN '2015-01-30' AND '2015-02-21'
//}






