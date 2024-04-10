#pragma once

#include <QtWidgets/QWidget>
#include "ui_Stagechoice1.h"

class Stagechoice1 : public QWidget
{
    Q_OBJECT

public:
    Stagechoice1(QWidget *parent = nullptr);
    ~Stagechoice1();

private:
    Ui::Stagechoice1Class ui;
};
