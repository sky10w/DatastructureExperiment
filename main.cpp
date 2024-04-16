#include "./StageSelection/MyPushButton.h"
#include "./StageSelection/Stagechoice1.h"
#include "mainwindow.h"

#include "gameboard/gameboard.h"
#include <QLabel>
#include <QLayout>
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  gameboard w;

  w.setFixedSize(1280, 720);
  w.show();
  return a.exec();
}
