#pragma once

#include <QtWidgets/QWidget>
#include <QLabel>
#include <QLayout>
#include "ui_Stagechoice1.h"
#include "StageSelection/MyPushButton.h"

class Stagechoice1 : public QWidget
{
    Q_OBJECT

public:
    Stagechoice1(QWidget *parent = nullptr);
    ~Stagechoice1();

private:
    Ui::Stagechoice1Class ui;
};
