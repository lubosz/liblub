#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "Renderer/FrameBuffer.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Planets/Planet.h"

class GLWidget: public QGLWidget {
  Q_OBJECT

public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  Planet* focusedPlanet;
  vector<Planet*> planets;

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

private:
  QPoint lastPos;
  Material *HDR;
  bool useHDR;
  Camera* camera;
  Light * light;
  FrameBuffer *fbo;

  void initCamAndLight();
  void initPostProcessing();
  void startPass();
  void endPass();
  void drawPlanets();
};

#endif
