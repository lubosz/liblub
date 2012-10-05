#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "Renderer/OpenGL.h"
#include <QGLWidget>

class GLWidget: public QGLWidget {
  Q_OBJECT

public:
  GLWidget(const QGLFormat & format, QWidget *parent);
  ~GLWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  QSize viewSize;
  void paintGL();

protected:
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void mouseReleaseEvent(QMouseEvent * event);
  QPoint lastMousePosition;
  float inputSpeed, mouseSensitivity;

signals:
  void draw();
};

#endif
