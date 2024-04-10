#include "mypushbutton.h"
//MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
//{

//}
void MyPushButton::move1()
{
    QPixmap map;
    map.load(this->press_img);
    this->setIcon(map);

    QPoint w = this->pos();
    w.setY(w.y() - 5);
    this->move(w);
}

void MyPushButton::move2()
{
    QPixmap map;
    map.load(this->normal_img);
    this->setIcon(map);

    QPoint w = this->pos();
    w.setY(w.y() + 5);
    this->move(w);
}

MyPushButton::MyPushButton(QString nor, QString pre)
{
    this->normal_img = nor;
    this->press_img = pre;

    QPixmap map;
    map.load(nor);
    //设置图片大小
    this->setFixedSize(map.width(),map.height());
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图片
    this->setIcon(map);
    this->setIconSize(QSize(map.width(), map.height()));
}
