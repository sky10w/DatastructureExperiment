#include "widget.h"

widget::widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget)
{
    ui->setupUi(this);

    this->setFixedSize(1280,720);

    mGameView.setSceneRect(QRect(0,0,1280,720));
    mScene.setSceneRect(QRect(0,0,1280,720));
    mStartScene.setSceneRect(QRect(0,0,1280,720));
    mSetScene.setSceneRect(QRect(0,0,1280,720));
    mStartScene.addPixmap(QPixmap(":/img/Image/bg1.jpg"));
    mSetScene.addPixmap(QPixmap(":/img/Image/MSI_MPG.jpg"));

    auto startBtn = new QToolButton();
    startBtn->setIcon(QIcon(":/img/Image/start1.png"));
    startBtn->setIconSize(QSize(215,98));
    startBtn->move(492,400);
    startBtn->setAutoRaise(true);

    connect(startBtn,&QToolButton::clicked,[this](){
        mGameView.setScene(&mScene);
        mGameView.show();
    });

    mStartScene.addWidget(startBtn);



    auto startBtn1 = new QToolButton();
    startBtn1->setIcon(QIcon(":/img/Image/sett.png"));
    startBtn1->setIconSize(QSize(69,66));
    startBtn1->move(1211,0);
    startBtn1->setAutoRaise(true);

    connect(startBtn1,&QToolButton::clicked,[this](){
        mGameView.setScene(&mSetScene);
        mGameView.show();
    });

    mStartScene.addWidget(startBtn1);







    mBackGround1.setPixmap(QPixmap(":/img/Image/map1.png"));
 //   mStart.setPixmap(QPixmap(":/img/Image/start1.png"));

 //   mStart.setPos(492,400);

    mScene.addItem(&mBackGround1);

    mGameView.setScene(&mStartScene);
    mGameView.setParent(this);
    mGameView.show();
}


widget::~widget()
{
    delete ui;
}

void widget::on_pushButton_clicked()
{
    //this->close();
    mGameView.show();

    login1 *pic = new login1();
    pic->show();

}


void widget::on_pushButton_2_clicked()
{
    this->close();
    setting *pic = new setting();
    pic->show();
}
void widget::on_pushButton_3_clicked()
{

}


