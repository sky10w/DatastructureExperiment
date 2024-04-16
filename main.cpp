#include "mainwindow.h"
#include "StageSelection/Stagechoice1.h"
#include "InGame/System.h"

#include <QApplication>
#include <QLabel>
#include <QLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BuffSystem::initBuff();
    CardSystem::initCard();

    // MainWindow w;
    Stagechoice1 w;
    w.setFixedSize(1280, 720);
    w.show();



    return a.exec();
}
