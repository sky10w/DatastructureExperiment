#include "Stagechoice1.h"
<<<<<<< HEAD
#include"levelgenerator.h"

Stagechoice1::Stagechoice1(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    LevelGenerator generator;
    for(int i = 0;i<9;i++){
        btn.push_back(new MyPushButton (i,QString(":/Stagechoice1/res/image/battle1.png"), QString(":/Stagechoice1/res/image/battle2.png")));
        btn[i]->setParent(this);
        connect(btn[i], &MyPushButton::pressed, [=]() {
            btn[i]->move1();
        });
        connect(btn[i], &MyPushButton::released, [=]() {
            btn[i]->move2();
        });
        connect(btn[i],&MyPushButton::clicked,btn[i],&MyPushButton::nosend);
        connect(btn[i],SIGNAL(nosended(int)),this,SLOT(levelChoose(int)));

        connect(btn[i],&MyPushButton::clicked,btn[i],&MyPushButton::nosend);
        connect(btn[i],SIGNAL(nosended(int)),this,SLOT(levelClear(int)));
        //目前上面两个都是点击触发（点关和通关）
    }

    connect(&generator, &LevelGenerator::levelGenerated, this, &Stagechoice1::handleLevelGenerated);
    generator.generateLevel();

     //10是boss按钮11是返回按钮12是设置按钮
    btnel.push_back(new MyPushButton (10,QString(":/Stagechoice1/res/image/boss1.png"), QString(":/Stagechoice1/res/image/boss2.png")));
    btnel.push_back(new MyPushButton (11,QString(":/Stagechoice1/res/image/back1.png"), QString(":/Stagechoice1/res/image/back2.png")));
    btnel.push_back(new MyPushButton (12,QString(":/Stagechoice1/res/image/settings1.png"), QString(":/Stagechoice1/res/image/settings2.png")));
    for(int i = 0;i<3;i++){
        btnel[i]->setParent(this);
        connect(btnel[i], &MyPushButton::pressed, [=]() {
            btnel[i]->move1();
        });
        connect(btnel[i], &MyPushButton::released, [=]() {
            btnel[i]->move2();
        });
    }

    btnel[0]->move(970,330);
    btnel[1]->move(60,60);
    btnel[2]->move(1140,55);

}

Stagechoice1::~Stagechoice1()
{}

void Stagechoice1::drawDashedLine(QPoint start,QPoint end) {
    QPainter painter(this); // 创建一个绘图对象，并将其与当前窗口关联
    QPoint px = start;
    QPoint py = end;
    px.setX(start.x()+30);
    px.setY(start.y()+30);
    py.setX(end.x()+30);
    py.setY(end.y()+30);
    painter.setPen(Qt::black); // 设置画笔颜色为黑色
    painter.setRenderHint(QPainter::Antialiasing, true); // 设置抗锯齿渲染
    painter.setPen(Qt::DashLine); // 设置画笔样式为虚线
    painter.drawLine(px, py); // 绘制虚线
}


void Stagechoice1::handleLevelGenerated(QList<lnode> nodes, QList<QPair<int, int>> edges,int branchno) {
    currentnode = 0;
    nodesm = nodes;
    edgesm = edges;
    branch = branchno;
    for(int i = 0;i<9;i++){

        btn[i]->move(nodes[i].position);
    }

}

void Stagechoice1::levelClear(int node){
    currentnode = node;
    update();
}
void Stagechoice1::levelChoose(int node){

}

=======

Stagechoice1::Stagechoice1(QWidget *parent) : QWidget(parent) {
  // ui.setupUi(this);
  MyPushButton *btn1 =
      new MyPushButton(QString(":/StageSelection/res/image/back1.png"),
                       QString(":/StageSelection/res/image/back2.png"));
  MyPushButton *btn2 =
      new MyPushButton(QString(":/StageSelection/res/image/battle1.png"),
                       QString(":/StageSelection/res/image/battle2.png"));
  MyPushButton *btn3 =
      new MyPushButton(QString(":/StageSelection/res/image/battle1.png"),
                       QString(":/StageSelection/res/image/battle2.png"));
  MyPushButton *btn4 =
      new MyPushButton(QString(":/StageSelection/res/image/boss1.png"),
                       QString(":/StageSelection/res/image/boss2.png"));
  MyPushButton *btn5 =
      new MyPushButton(QString(":/StageSelection/res/image/settings1.png"),
                       QString(":/StageSelection/res/image/settings2.png"));
  // MyPushButton *button = new MyPushButton();
  // button->setParent(this);

  btn1->setParent(this);
  btn2->setParent(this);
  btn3->setParent(this);
  btn4->setParent(this);
  btn5->setParent(this);

  btn1->move(40, 40);
  connect(btn1, &MyPushButton::pressed, [=]() { btn1->move1(); });
  connect(btn1, &MyPushButton::released, [=]() { btn1->move2(); });
  btn2->move(415, 455);
  connect(btn2, &MyPushButton::pressed, [=]() { btn2->move1(); });
  connect(btn2, &MyPushButton::released, [=]() { btn2->move2(); });
  btn3->move(605, 370);
  connect(btn3, &MyPushButton::pressed, [=]() { btn3->move1(); });
  connect(btn3, &MyPushButton::released, [=]() { btn3->move2(); });
  btn4->move(770, 200);
  connect(btn4, &MyPushButton::pressed, [=]() { btn4->move1(); });
  connect(btn4, &MyPushButton::released, [=]() { btn4->move2(); });
  btn5->move(1160, 40);
  connect(btn5, &MyPushButton::pressed, [=]() { btn5->move1(); });
  connect(btn5, &MyPushButton::released, [=]() { btn5->move2(); });

  QLabel *backgroundLabel = new QLabel(this);
  backgroundLabel->setObjectName("backgroundLabel");
  backgroundLabel->setScaledContents(true);
  this->setStyleSheet("QLabel#backgroundLabel {"
                      "  border-image: url(:/StageSelection/res/image/map.png);"
                      "}");
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(backgroundLabel);
  this->setLayout(layout);
  backgroundLabel->lower();
}

Stagechoice1::~Stagechoice1() {}
>>>>>>> wy_
