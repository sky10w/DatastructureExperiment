#include "myopenglwidget.h"
#include <QOpenGLFunctions>

void MyOpenGLWidget::initializeGL() {
  // 初始化OpenGL函数
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MyOpenGLWidget::paintGL() {
  // 清除颜色缓冲区
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glClear(GL_COLOR_BUFFER_BIT);
}

void MyOpenGLWidget::resizeGL(int width, int height) {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glViewport(0, 0, width, height);
}
