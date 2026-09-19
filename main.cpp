#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set the application icon.
    a.setWindowIcon(QIcon(":/notex.png"));
    a.setDesktopFileName("notex");
    MainWindow w;
    w.show();
    return QApplication::exec();
}
