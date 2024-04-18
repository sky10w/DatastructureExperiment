#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "Title/widget.h"
#include "StageSelection/Stagechoice1.h"
#include "InGame/System.h"


QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum GameScene : int
    {
        TITLE = 0,
        STAGECHOICE,
        INGAME
    };


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void enterStage(bool isBoss);
    void changeScene(int scene);

private:
    widget* _title;
    Stagechoice1* _stage;
    InGameSystem* _ingame;
    QGraphicsScene* _scene[3];
    QGraphicsView* _view;
    int _nowScene;
    bool _isBoss;

};
#endif // MAINWINDOW_H
