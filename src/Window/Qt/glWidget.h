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
  Planet* earth;

public slots:
  void setRed(int red);
  void setGreen(int red);
  void setBlue(int red);
  void setAttenuation(bool attenuation);

signals:
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

private:
  QVector3D earthWaveLength;
  QPoint lastPos;

  Material *HDR;
  bool useHDR;
  Camera* camera;
  Light * light;
  FrameBuffer *fbo;
  vector<Planet*> planets;

  void initCamAndLight();
  void initPostProcessing();
  void startPass();
  void endPass();
  void drawPlanets();
};

#endif
