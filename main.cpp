#include "widget.h"

#include <QApplication>
#include "writewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

//    WriteWidget w(QString("1"), 8);
//    w.show();

    return a.exec();
}
