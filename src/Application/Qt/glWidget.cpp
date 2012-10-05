#include <QtGui>
#include "glWidget.h"
#include "Scene/Scene.h"
#include "QtInput.h"

#include "System/Config.h"

GLWidget::GLWidget(const QGLFormat & format, QWidget *parent) :
  QGLWidget(format, parent) {
  LogDebug << "Initializing Qt GL Wiget";
  inputSpeed = Config::Instance().value<float>("inputSpeedSlow");
  mouseSensitivity = Config::Instance().value<float>("mouseSensitivity");
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
  Scene::Instance().getCurrentCamera()->setAspect(
      float(width) / float(height));
  glViewport(0, 0, width, height);
  viewSize = QSize(width, height);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
      lastMousePosition = event->pos();
    }
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0)
        Scene::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
    else
        Scene::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
    event->accept();
    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  this->setFocus();

  if (event->buttons() & Qt::LeftButton) {
    int dx = event->x() - lastMousePosition.x();
    int dy = event->y() - lastMousePosition.y();
    QtInput::Instance().mousePosition = QVector2D(dx,dy);
    QtInput::Instance().move();
    lastMousePosition = event->pos();
  }
}


void GLWidget::mouseReleaseEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        QtInput::Instance().mousePosition = QVector2D();
    }
}
