#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QToolButton>

#include "login1.h"
#include "setting.h"


class widget : public QWidget
{
    Q_OBJECT

public:
    widget(QWidget *parent = nullptr);
    ~widget();
    QGraphicsScene *getScene() const;
    void showView();
    void closeView();

signals:
    void changeScene(int scene);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    QGraphicsView *mGameView;
    QGraphicsScene mScene;
    QGraphicsScene *mStartScene;//开始菜单
    QGraphicsScene mSetScene;//设置菜单
    QGraphicsPixmapItem mStart;
    QGraphicsPixmapItem mBackGround1;
    QGraphicsPixmapItem mBackGround2;

};
#endif // WIDGET_H
