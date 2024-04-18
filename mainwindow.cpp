#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _isBoss(false)
      // , _title(nullptr)
      // , _stage(nullptr)
      // , _ingame(nullptr)
      ,
      _title(new widget()), _stage(new Stagechoice1()),
      _ingame(new InGameSystem()), _view(new QGraphicsView(this)) {
  // _title->setContentsMargins({0, 0, 0, 0});
  // _stage->setContentsMargins({0, 0, 0, 0});
  // _ingame->setContentsMargins({0, 0, 0, 0});
  QObject::connect(_ingame, SIGNAL(changeScene(int)), this,
                   SLOT(changeScene(int)));
  QObject::connect(_title, SIGNAL(changeScene(int)), this,
                   SLOT(changeScene(int)));
  QObject::connect(_stage, SIGNAL(enterStage(bool)), this,
                   SLOT(enterStage(bool)));

  for (auto &i : this->_scene) {
    i = new QGraphicsScene();
    i->setSceneRect(QRect(0, 0, 1280, 720));
  }
  _scene[0]->addWidget(_title);
  _scene[1]->addWidget(_stage);
  _scene[2]->addWidget(_ingame);

  _view->setMinimumSize(1280, 720);
  _view->setFrameShape(QFrame::NoFrame);
  _view->setFrameRect({0, 0, 0, 0});
  this->changeScene(0);
  _view->show();
}

MainWindow::~MainWindow() {
  this->_view->close();
  this->_view->deleteLater();
}

void MainWindow::enterStage(bool isBoss) {
  this->_isBoss = isBoss;
  changeScene(INGAME);
}

void MainWindow::changeScene(int scene) {
  this->_view->setScene(this->_scene[scene]);
  if (scene == INGAME) {
    _ingame->initSystem(this->_isBoss);
    _ingame->run();
  }
}
