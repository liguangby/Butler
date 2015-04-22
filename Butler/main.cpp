#include "boot.h"
#include <QApplication>
#include "login.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Boot w;
    //Login w;
    w.show();
    return a.exec();
}
