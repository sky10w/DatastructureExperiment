#include "InGame/System.h"
#include "StageSelection/Stagechoice1.h"
#include "Title/widget.h"
#include "gameboard/gameboard.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLabel>
#include <QLayout>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  BuffSystem::initBuff();
  CardSystem::initCard();

  // MainWindow w;
  /*
  gameboard w;
  w.setFixedSize(1280, 720);
  w.show();
  */
  // InGameSystem *x = new InGameSystem(false);
  // x->run();
  // widget w;
  // w.show();
  MainWindow *w = new MainWindow(nullptr);
  w->resize(1280, 720);
  w->show();
  return a.exec();
}
