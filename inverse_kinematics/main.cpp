#include <QtGui/QApplication>
#include "mainwindow.h"
#include "glwidget.h"
#include <QLayout>

/*
class A
{
public:
    int a;
    virtual int fun()
    {
        return 1;
    }
};

class B:public A
{
public:
    int b;

    virtual int fun()
    {
        return 7;
    }

};*/


/* Git test 1 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*A* p_A;
    p_A=new B();
    int r;
    r=p_A->fun();*/
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);
    MainWindow w;
    w.show();
    return a.exec();
}
