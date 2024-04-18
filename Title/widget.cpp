#include "widget.h"

widget::widget(QWidget *parent)
    : QWidget(parent), mStartScene(new QGraphicsScene()),
      mGameView(new QGraphicsView()) {
  this->setFixedSize(1280, 720);

  mGameView->setSceneRect(QRect(0, 0, 1280, 720));
  mScene.setSceneRect(QRect(0, 0, 1280, 720));
  mStartScene->setSceneRect(QRect(0, 0, 1280, 720));
  mSetScene.setSceneRect(QRect(0, 0, 1280, 720));
  mStartScene->addPixmap(QPixmap(":/img/Image/bg1.jpg"));
  mSetScene.addPixmap(QPixmap(":/img/Image/MSI_MPG.jpg"));
  mBackGround1.setPixmap(QPixmap(":/img/Image/map1.png"));
  mScene.addItem(&mBackGround1);

  auto startBtn = new QToolButton();

  startBtn->setStyleSheet("background:transparent");
  startBtn->setIcon(QIcon(":/img/Image/start1.png"));
  startBtn->setIconSize(QSize(215, 98));
  startBtn->move(492, 500);
  startBtn->setAutoRaise(true);

  QObject::connect(startBtn, &QToolButton::clicked, this, [this](bool) {
    qDebug() << "StartButton clicked";
    emit this->changeScene(1);
  });

  mStartScene->addWidget(startBtn);

  auto startBtn1 = new QToolButton();
  startBtn1->setIcon(QIcon(":/img/Image/sett.png"));
  startBtn1->setIconSize(QSize(69, 66));
  startBtn1->move(1211, 0);
  startBtn1->setAutoRaise(true);

  QObject::connect(startBtn1, &QToolButton::clicked, this, [this]() {
    mGameView->setScene(&mSetScene);
    mGameView->show();
  });

  mStartScene->addWidget(startBtn1);

  mGameView->setInteractive(true);
  mGameView->setScene(mStartScene);
  mGameView->setParent(this);
}

widget::~widget() {
  delete mGameView;
  delete mStartScene;
}

QGraphicsScene *widget::getScene() const { return this->mStartScene; }

void widget::showView() { this->mGameView->show(); }

void widget::closeView() { this->mGameView->close(); }

void widget::on_pushButton_clicked() {
  // this->close();
  mGameView->hide();
  emit this->changeScene(1);

  login1 *pic = new login1(this->parentWidget());
  pic->show();
}

void widget::on_pushButton_2_clicked() {
  this->close();
  setting *pic = new setting(this->parentWidget());
  pic->show();
}
void widget::on_pushButton_3_clicked() {}
