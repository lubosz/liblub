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

class GLWidget: public QGLWidget {
  Q_OBJECT

public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  QSize viewSize;
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
  bool usePostprocessing;
  bool useWireframe;
  Camera* camera;
  Light * light;
  FrameBuffer *fbo;

  void timerEvent(QTimerEvent *) { update(); }
  int timerId;


  void initCamAndLight();
  void initPostProcessing();
  void updatePostProcessing();
  void startPass();
  void endPass();
  void drawPlanets();
public slots:
  void setExposure(double exposure);
  void setLazy(bool lazy);
  void setWireframe(bool wire);
  void setPostprocessing(bool post);
};

#endif
