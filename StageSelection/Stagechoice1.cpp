#include "Stagechoice1.h"

Stagechoice1::Stagechoice1(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    MyPushButton* btn1 = new MyPushButton(QString(":/Stagechoice1/src/image/back1.png"),QString(":/Stagechoice1/src/image/back2.png"));
    MyPushButton* btn2 = new MyPushButton(QString(":/Stagechoice1/src/image/battle1.png"), QString(":/Stagechoice1/src/image/battle2.png"));
    MyPushButton* btn3 = new MyPushButton(QString(":/Stagechoice1/src/image/battle1.png"), QString(":/Stagechoice1/src/image/battle2.png"));
    MyPushButton* btn4 = new MyPushButton(QString(":/Stagechoice1/src/image/boss1.png"), QString(":/Stagechoice1/src/image/boss2.png"));
    MyPushButton* btn5 = new MyPushButton(QString(":/Stagechoice1/src/image/settings1.png"), QString(":/Stagechoice1/src/image/settings2.png"));
    btn1->setParent(this);
    btn2->setParent(this);
    btn3->setParent(this);
    btn4->setParent(this);
    btn5->setParent(this);
    btn1->move(40,40);
    connect(btn1, &MyPushButton::pressed, [=]() {
        btn1->move1();
        });
    connect(btn1, &MyPushButton::released, [=]() {
        btn1->move2();
        });
    btn2->move(415, 455);
    connect(btn2, &MyPushButton::pressed, [=]() {
        btn2->move1();
        });
    connect(btn2, &MyPushButton::released, [=]() {
        btn2->move2();
        });
    btn3->move(605, 370);
    connect(btn3, &MyPushButton::pressed, [=]() {
        btn3->move1();
        });
    connect(btn3, &MyPushButton::released, [=]() {
        btn3->move2();
        });
    btn4->move(770, 200);
    connect(btn4, &MyPushButton::pressed, [=]() {
        btn4->move1();
        });
    connect(btn4, &MyPushButton::released, [=]() {
        btn4->move2();
        });
    btn5->move(1160, 40);
    connect(btn5, &MyPushButton::pressed, [=]() {
        btn5->move1();
        });
    connect(btn5, &MyPushButton::released, [=]() {
        btn5->move2();
        });

    QLabel* backgroundLabel = new QLabel(this);
    backgroundLabel->setObjectName("backgroundLabel");
    backgroundLabel->setScaledContents(true);
    this->setStyleSheet("QLabel#backgroundLabel {"
                    "  border-image: url(:/Stagechoice1/src/image/map.png);"
                    "}");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(backgroundLabel);
    this->setLayout(layout);
    backgroundLabel->lower();
}

Stagechoice1::~Stagechoice1()
{}
