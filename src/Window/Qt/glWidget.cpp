#include <QtGui>
#include "glWidget.h"
#include "Scene/SceneData.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(const QGLFormat & format, QWidget *parent) :
  QGLWidget(format, parent) {
  LogDebug << "Initializing Qt GL Wiget";
}

GLWidget::~GLWidget() {
}

QSize GLWidget::minimumSizeHint() const {
  return QSize(800, 600);
}

QSize GLWidget::sizeHint() const {
  return QSize(1720, 1200);
}

void GLWidget::paintGL() {
  emit draw();
}

void GLWidget::resizeGL(int width, int height) {
  SceneData::Instance().getCurrentCamera()->setAspect(
      float(width) / float(height));
  glViewport(0, 0, width, height);
  viewSize = QSize(width, height);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastMousePosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  this->setFocus();

  if (event->buttons() & Qt::LeftButton) {
    int dx = event->x() - lastMousePosition.x();
    int dy = event->y() - lastMousePosition.y();
    SceneData::Instance().getCurrentCamera()->setMouseLook(dx, dy, .1);
    updateGL();
  } else if (event->buttons() & Qt::RightButton) {
  }
  lastMousePosition = event->pos();
}

