#ifndef GLWIDGET_H
#define GLWIDGET_H

#ifdef USE_GLEW
# include <GL/glew.h>
#endif
#include <QGLWidget>
#include "Renderer/FrameBuffer.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Planet.h"
#include "System/GUI.h"

class PlanetsApp;

class GLWidget: public QGLWidget {
  Q_OBJECT

public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();

  PlanetsApp * app;

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  QSize viewSize;
  void paintGL();
  void setApp(PlanetsApp * app);

protected:
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  QPoint lastMousePosition;

signals:
  void draw();
};

#endif
