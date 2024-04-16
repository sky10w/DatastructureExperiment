#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#include <QtOpenGLWidgets/QOpenGLWidget>

class MyOpenGLWidget : public QOpenGLWidget {
  Q_OBJECT

public:
  MyOpenGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
};

#endif // MYOPENGLWIDGET_H
