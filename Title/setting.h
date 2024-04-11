#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include "widget.h"

namespace Ui {
class setting;
}

class setting : public QWidget
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = nullptr);
    ~setting();

private slots:
    void on_pushButton_clicked();

private:
    Ui::setting *ui;


};

#endif // SETTING_H
