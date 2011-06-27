#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setGraphicsSystem("raster"); //added for performance improvement
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
