#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Ruszczak");
    a.setApplicationName("LogsFilter");
    a.setOrganizationDomain("pl.ruszczak");

    MainWindow w;
    w.show();

    return a.exec();
}
