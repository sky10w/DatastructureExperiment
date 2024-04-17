#include "Stagechoice1.h"

Stagechoice1::Stagechoice1(QWidget* parent)
    : QWidget(parent)
{

  LevelGenerator generator;
  for (int i = 0; i < 9; i++) {
    btn.push_back(
        new MyPushButton(i, QString(":/Stagechoice1/res/image/battle1.png"),
                         QString(":/Stagechoice1/res/image/battle2.png")));
    btn[i]->setParent(this);
    connect(btn[i], &MyPushButton::pressed, [=]() { btn[i]->move1(); });
    connect(btn[i], &MyPushButton::released, [=]() { btn[i]->move2(); });
    connect(btn[i], &MyPushButton::clicked, btn[i], &MyPushButton::nosend);
    connect(btn[i], SIGNAL(nosended(int)), this, SLOT(levelChoose(int)));

    connect(btn[i], &MyPushButton::clicked, btn[i], &MyPushButton::nosend);
    connect(btn[i], SIGNAL(nosended(int)), this, SLOT(levelClear(int)));
    //目前上面两个都是点击触发（点关和通关）
  }

  connect(&generator, &LevelGenerator::levelGenerated, this,
          &Stagechoice1::handleLevelGenerated);
  generator.generateLevel();

  // 10是boss按钮11是返回按钮12是设置按钮
  btnel.push_back(
      new MyPushButton(10, QString(":/Stagechoice1/res/image/boss1.png"),
                       QString(":/Stagechoice1/res/image/boss2.png")));
  btnel.push_back(
      new MyPushButton(11, QString(":/Stagechoice1/res/image/back1.png"),
                       QString(":/Stagechoice1/res/image/back2.png")));
  btnel.push_back(
      new MyPushButton(12, QString(":/Stagechoice1/res/image/settings1.png"),
                       QString(":/Stagechoice1/res/image/settings2.png")));
  for (int i = 0; i < 3; i++) {
    btnel[i]->setParent(this);
    connect(btnel[i], &MyPushButton::pressed, [=]() { btnel[i]->move1(); });
    connect(btnel[i], &MyPushButton::released, [=]() { btnel[i]->move2(); });
  }

  btnel[0]->move(970, 330);
  btnel[1]->move(60, 60);
  btnel[2]->move(1140, 55);
}

Stagechoice1::~Stagechoice1() {}

void Stagechoice1::drawDashedLine(QPoint start, QPoint end) {
  QPainter painter(this); // 创建一个绘图对象，并将其与当前窗口关联
  QPoint px = start;
  QPoint py = end;
  px.setX(start.x() + 30);
  px.setY(start.y() + 30);
  py.setX(end.x() + 30);
  py.setY(end.y() + 30);
  painter.setPen(Qt::black); // 设置画笔颜色为黑色
  painter.setRenderHint(QPainter::Antialiasing, true); // 设置抗锯齿渲染
  painter.setPen(Qt::DashLine); // 设置画笔样式为虚线
  painter.drawLine(px, py);     // 绘制虚线
}

void Stagechoice1::handleLevelGenerated(QList<lnode> nodes,
                                        QList<QPair<int, int>> edges,
                                        int branchno) {
  currentnode = 0;
  nodesm = nodes;
  edgesm = edges;
  branch = branchno;
  for (int i = 0; i < 9; i++) {

    btn[i]->move(nodes[i].position);
  }
}

void Stagechoice1::levelClear(int node) {
  currentnode = node;
  update();
}
void Stagechoice1::levelChoose(int node) {}
