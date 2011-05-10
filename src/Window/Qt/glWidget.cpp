#include <QtGui>
#include <math.h>

#include "glWidget.h"
#include "System/GUI.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "Renderer/RenderEngine.h"
#include "System/Timer.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
  xRot = 0;
  yRot = 0;
  zRot = 0;

  qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
  qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

  QGLFormat fmt;
  fmt.setVersion(4,1);
//  fmt.setAlpha(true);
//  fmt.setStereo(true);
  QGLFormat::setDefaultFormat(fmt);
}

GLWidget::~GLWidget() {
}

QSize GLWidget::minimumSizeHint() const {
  return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
  return QSize(400, 400);
}

static void qNormalizeAngle(int &angle) {
  while (angle < 0)
    angle += 360 * 16;
  while (angle > 360 * 16)
    angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != xRot) {
    xRot = angle;
    SceneData::Instance().getCurrentCamera()->yaw = xRot;
    SceneData::Instance().getCurrentCamera()->updateRotation();
    emit
    xRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::setYRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != yRot) {
    yRot = angle;
    SceneData::Instance().getCurrentCamera()->pitch = yRot;
    SceneData::Instance().getCurrentCamera()->updateRotation();
    emit
    yRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::setZRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != zRot) {
    zRot = angle;
    SceneData::Instance().getCurrentCamera()->roll = zRot;
    SceneData::Instance().getCurrentCamera()->updateRotation();
    emit
    zRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::initializeGL() {
  Config::Instance().load("config.xml");
  SceneLoader * sceneLoader = new SceneLoader("nice.xml");
  sceneLoader->load();
  GUI::Instance().init();
}

void GLWidget::paintGL() {
  RenderEngine::Instance().clear();
  SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
  Timer::Instance().frame();
}

void GLWidget::resizeGL(int width, int height) {
//  int side = qMin(width, height);
  SceneData::Instance().getCurrentCamera()->setAspect(float(width) / float(height));
  glViewport(0, 0, width,height);
//  LogDebug << side;
//  glViewport((width - side) / 2, (height - side) / 2, side, side);
//  glMatrixMode( GL_PROJECTION);
//  glLoadIdentity();
//  glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
//  glMatrixMode( GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(xRot + 8 * dy);
    setYRotation(yRot + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(xRot + 8 * dy);
    setZRotation(zRot + 8 * dx);
  }
  lastPos = event->pos();
}
