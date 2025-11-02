#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("HTML to PDF Converter");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("YourCompany");

    MainWindow w;
    w.show();
    return a.exec();
}
