#include "add_user_data.h"
#include "ui_add_user_data.h"
Add_user_data::Add_user_data(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_user_data)
{
    ui->setupUi(this);
    SETICON;
    timeid = new QTimer(this);
    connect(timeid,SIGNAL(timeout()),this,SLOT(timedate()));    //获取系统时间
    timeid->start(1000);
    tag1 = true;
    QDate the_data;
    QTime the_time;
    QString date = the_data.currentDate().toString("yyyy-MM-dd");
    QString the_number = the_data.currentDate().toString("yyyyMMdd");
    the_number += the_time.currentTime().toString("hhmmss");
    ui->text_number->setText(the_number);
    ui->time->setText(date);
    calen = NULL;
    movie = new QMovie("://sources/cow.gif");
    ui->movie->setMovie(movie);
    movie->start();
    ///////////////*****************************************////设置金钱栏只能输入数字，且最多九位
    QValidator *validator=new QIntValidator(0,999999999,this);
    ui->Add_money->setValidator(validator);
    ///***************************************************/
    ui->type->setStyleSheet("QComboBox{border-image:url(:/add_user_data/images/type.png);}");
    ui->money_type->setStyleSheet("QComboBox{border-image:url(:/add_user_data/images/type.png);}");
    ui->Add_money->setStyleSheet("QLineEdit{border-image:url(:/add_user_data/images/money.png);}");
    ui->Add_backup->setStyleSheet("QTextEdit{border-image:url(:/add_user_data/images/backup.png);}");
    ui->time->setStyleSheet("QPushButton{border-image:url(:/add_user_data/images/time.png);}");
    ui->Add_save->setStyleSheet("QPushButton{border-image:url(:/add_user_data/images/add_1.png);}QPushButton:hover{border-image:url(:/add_user_data/images/add_0.png);}QPushButton:pressed{border-image:url(:/add_user_data/images/add_1.png);}");
    ui->Add_cancle->setStyleSheet("QPushButton{border-image:url(:/add_user_data/images/cancle_1.png);}QPushButton:hover{border-image:url(:/add_user_data/images/cancle_0.png);}QPushButton:pressed{border-image:url(:/add_user_data/images/cancle_1.png);}");
    ui->close->setStyleSheet("QPushButton{border-image:url(:/images/close_0.png);}QPushButton:hover{border-image:url(:/images/close_1.png);}QPushButton:pressed{border-image:url(:/images/close_0.png);}");
}
/*****************隐藏标题栏可以移动代码*********************/
void Add_user_data::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}

void Add_user_data::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    move(x()+dx, y()+dy);
}

void Add_user_data::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    move(x()+dx, y()+dy);
}
/***********************************************/
//析构函数
Add_user_data::~Add_user_data()
{
    movie->stop();
    delete calen;
    calen = NULL;
    delete ui;
}
//更新时间函数
void Add_user_data::timedate()
{
    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    currentDate = dt.toString("yyyyMMddhhmmss");
}
//关闭按钮
void Add_user_data::on_Add_cancle_clicked()
{
    movie->stop();
    delete calen;
    calen = NULL;
    this->close();
}
//保存按钮
void Add_user_data::on_Add_save_clicked()
{
    if(ui->Add_money->text() == NULL)
    {
        QMessageBox::information(this,"提示","金额不能为空",QMessageBox::Yes);
    }
    else if(!(ui->save_in->isChecked())&&!(ui->save_out->isChecked()))
    {
        QMessageBox::information(this,"警告","请选择收支类型",QMessageBox::Yes);
    }
    else
    {
        movie->stop();
        add_thedata_todata();
        this->close();
    }
}
//调出日历
void Add_user_data::on_time_clicked()
{
    calen = new QCalendarWidget(this);
    calen->move(320,60);
    calen->resize(250,200);
    calen->show();
    calen->setSelectionMode(QCalendarWidget::SingleSelection);
    QObject::connect(calen,SIGNAL(clicked(const QDate &)),this,SLOT(setdata()));
}
//添加日期函数
void Add_user_data::setdata()
{
    QDate datar = calen->selectedDate();
    QString date = datar.toString("yyyy-MM-dd");
    ui->time->setText(date);
    calen->hide();
}
/****************收支选择****************************/
void Add_user_data::on_save_in_clicked(bool checked)
{
    if(checked)
    {
        ui->save_out->setChecked(false);
        tag1 = true;
        combobox_in();
    }
}

void Add_user_data::on_save_out_clicked(bool checked)
{
    if(checked)
    {
        ui->save_in->setChecked(false);
        tag1 = false;
        combobox_out();
    }
}
/****************************************************/
//检测
void Add_user_data::on_type_editTextChanged(const QString &arg1)
{
    user_add_what = arg1;
}
void Add_user_data::on_money_type_currentTextChanged(const QString &arg1)
{
    user_add_tip = arg1;
}
//监测金钱输入
void Add_user_data::on_Add_money_textChanged(const QString &arg1)
{

    QString text = arg1.mid(0,1);
    if(text=="+")
    {
        ui->save_in->setChecked(true);
        ui->save_out->setChecked(false);
        tag1 = true;
        combobox_in();
    }
    else if(text=="-")
    {
        ui->save_in->setChecked(false);
        ui->save_out->setChecked(true);
        tag1 = false;
        combobox_out();
    }
    else
    {
        bool x = ui->save_out;    //如果不添加判断， 那么将会导致在用户先选择支出，在添加具体金额是，将支出改为收入
        if(!x)
        {
            tag1 = true;
        }
    }
}
//关闭函数
void Add_user_data::on_close_clicked()
{
    movie->stop();
    delete calen;
    calen = NULL;
    this->close();
}
//添加存入选项
void Add_user_data::combobox_in()
{
    ui->type->clear();
    ui->money_type->clear();
    QStringList data1,data2;
    data1<<"现金"<<"优惠卷"<<"支票"<<"银行转账";
    data2<<"公司工资"<<"公司奖金"<<"优惠券"<<"彩票"<<"别人赠送";
    ui->money_type->addItems(data1);
    ui->type->addItems(data2);
}
//添加支出选项
void Add_user_data::combobox_out()
{
    ui->type->clear();
    ui->money_type->clear();
    QStringList data1,data2;
    data1<<"现金"<<"优惠卷"<<"支票"<<"银行卡"<<"信用卡";
    data2<<"吃饭"<<"教育"<<"购物"<<"送礼"<<"生活"<<"保险";
    ui->money_type->addItems(data1);
    ui->type->addItems(data2);
}
//存入数据库
void Add_user_data::add_thedata_todata()
{
    userdataname = "User_Data_";
    userdata = dbopen.initDatabase("User_recode.db");
    QSqlQuery query(userdata);
    query.exec("select * from User_recode");
    while (query.last())
    {
        userdataname += query.value(1).toString();
        userdataname +=".db";
        dbopen.closedb();
        break;
    }
    QString usertip;
    if(tag1)
    {
        usertip = "存入";
    }
    else if(!tag1)
    {
        usertip = "支出";
    }
    userdata = dbopen.initDatabase(userdataname);
    QSqlQuery query1(userdata);
    user_add_number = ui->text_number->text();
    user_add_time = ui->time->text();
    user_add_money = ui->Add_money->text();
    user_add_remark = ui->Add_backup->toPlainText();
    QString querysql = "insert into UserData values('";
    querysql += user_add_number;
    querysql += "','";
    querysql += user_add_time;
    querysql += "','";
    querysql += usertip;
    querysql += "','";
    querysql += user_add_what;
    querysql += "','";
    querysql += user_add_money;
    querysql += "','";
    querysql += user_add_tip;
    querysql += "','";
    querysql += user_add_remark;
    querysql += "')";
    query1.exec(querysql);
    dbopen.closedb();
    accept();
}



