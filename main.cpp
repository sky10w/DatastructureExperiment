#include "mainwindow.h"
#include "Stagechoice1.h"

#include <QApplication>
#include <QLabel>
#include <QLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    Stagechoice1 w;



    w.setFixedSize(1280, 720);
    w.show();
    return a.exec();
}
