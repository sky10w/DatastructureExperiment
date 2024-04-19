#pragma once

#include "StageSelection/MyPushButton.h"
#include "StageSelection/levelgenerator.h"
#include "ui_Stagechoice1.h"
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QPair>
#include <QVector>
#include <QtWidgets/QWidget>

class Stagechoice1 : public QWidget {
  Q_OBJECT

public:
  QVector<MyPushButton *> btn;
  QVector<MyPushButton *> btnel;

  QList<lnode> nodesm;
  QList<QPair<int, int>> edgesm;
  int branch;
  int currentnode;
  int bossable = 0;
  int chosen;

  void drawDashedLine(QPoint start, QPoint end);
  Stagechoice1(QWidget *parent = nullptr);
  ~Stagechoice1();

signals:
  void enterStage(bool isBossLevel);

public slots:
  void handleLevelGenerated(QList<lnode> nodes, QList<QPair<int, int>> edges,
                            int branchno);
  void levelClear(int node);  //通关处理槽函数
  void levelChoose(int node); //关卡点击处理槽函数

protected:
  void paintEvent(QPaintEvent *event) override {
    // 绘制背景
    QPainter painter(this);
    QPixmap background = QPixmap(":/Stagechoice1/res/image/map.png");
    painter.drawPixmap(0, 0, 1280, 720, background);

    for (int i = 0; i < edgesm.size(); i++) {
      if (currentnode == branch + 1 || currentnode == branch + 2) {
        chosen = currentnode;
      }
      if (currentnode == 8) {
        bossable = 1;
      }
      if (edgesm[i].first <= currentnode) {
        qDebug() << "chosen:" << chosen;
        if (i >= 2 && edgesm[i].second - 3 != branch) {
          drawDashedLine(nodesm[edgesm[i].first].position,
                         nodesm[edgesm[i].second].position);
        } else if (i < 2) {
          drawDashedLine(nodesm[edgesm[i].first].position,
                         nodesm[edgesm[i].second].position);
        } else if (edgesm[i].first == chosen) {
          drawDashedLine(nodesm[edgesm[i].first].position,
                         nodesm[edgesm[i].second].position);
        } else if (bossable == 1) {
          // drawDashedLine(QPoint(900, 340), QPoint(1000, 340));
        }
      }
    }
  }

private:
  QGraphicsView *view;
  // private:
  // Ui::Stagechoice1 ui;
};
