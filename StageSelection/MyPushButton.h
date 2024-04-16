#pragma once
#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QPoint>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //    explicit MyPushButton(QWidget *parent = nullptr);
        //构造函数,参数:正常的图片路径,按下后的图片路径
    MyPushButton(int btnno,QString normal, QString press = "");
    int btnid;
    QString normal_img;//正常路径
    QString press_img;//按下后的路径
    void move1();//点击上移
    void move2();//松开下移

signals:
    int nosended(int btnid);

public slots:
    void nosend();
};

#endif // MYPUSHBUTTON_H

