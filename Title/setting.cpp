#include "setting.h"
#include "ui_setting.h"

setting::setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting)
{
    ui->setupUi(this);
}

setting::~setting()
{
    delete ui;
}

void setting::on_pushButton_clicked()
{
    widget *father =new widget();
    this->close();
    father->show();
}

