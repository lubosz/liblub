/*
 * PlanetsApp.h
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#pragma once

#include "Renderer/OpenGL.h"
#include <QApplication>
#include <QtGui>
#include "Application/Qt/QtWindow.h"
#include "Application/Qt/glWidget.h"
#include "System/FontOverlay.h"

class QtApplication: public QApplication {

Q_OBJECT

public:
  GLWidget *glWidget;
  FontOverlay* fontOverlay;
  QtWindow * window;
  QTimer *drawTimer;
  bool useFontOverlay;

  bool useWireframe;

  void run();

  QtApplication(int &argc, char **argv);
  ~QtApplication();

  virtual void renderFrame() = 0;
  virtual void scene() = 0;

public slots:
  void setWireframe(bool wire);
  void draw();
  void setLazy(bool lazy);
};
