#include <QApplication>
#include "mainwindow.h"
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


/* Git test 3 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(4);
    QGLFormat::setDefaultFormat(glf);
*/
    /*A* p_A;
    p_A=new B();
    int r;
    r=p_A->fun();*/
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    //QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForTr(codec);
    MainWindow w;
    w.show();
    return a.exec();
}
