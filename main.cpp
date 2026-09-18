#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set the application icon.
    a.setWindowIcon(QIcon(":/marktek.png"));
    MainWindow w;
    w.show();
    return QApplication::exec();
}
