#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("CodeThesis");
    a.setApplicationName("TextReplacePro");
    MainWindow w;
    w.show();

    return a.exec();
}
