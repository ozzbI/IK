#include <QApplication>
#include "mainwindow.h"
#include <QLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGLFormat glf = QGLFormat::defaultFormat();
    //glf.setSampleBuffers(true);
    glf.setProfile(QGLFormat::CompatibilityProfile);
    glf.setVersion(2,1);
    //glf.setSamples(4);
    QGLFormat::setDefaultFormat(glf);

    +    /*
    +     * 1
    +     *
    +     * 3
    +     *
    +     */

    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    //QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForTr(codec);
    MainWindow w;

    //w.glw->makeCurrent();

    w.show();
    return a.exec();
}
