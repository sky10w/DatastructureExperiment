#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <login1.h>
#include <setting.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class widget;
}
QT_END_NAMESPACE



class widget : public QWidget
{
    Q_OBJECT

public:
    widget(QWidget *parent = nullptr);
    ~widget();



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::widget *ui;

    QGraphicsView mGameView;
    QGraphicsScene mScene;
    QGraphicsScene mStartScene;//开始菜单
    QGraphicsScene mSetScene;//设置菜单
    QGraphicsPixmapItem mStart;
    QGraphicsPixmapItem mBackGround1;
    QGraphicsPixmapItem mBackGround2;

};
#endif // WIDGET_H
