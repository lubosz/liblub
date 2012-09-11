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
  QPoint lastMousePosition;

signals:
  void draw();
};

#endif
