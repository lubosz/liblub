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
#include "Window/Qt/QtWindow.h"
#include "Window/Qt/glWidget.h"
#include "System/GUI.h"


class QtApplication: public QObject {

Q_OBJECT

public:
  GLWidget *glWidget;
  GUI* gui;
  QtWindow * window;
  QTimer *drawTimer;
  bool fontOverlay;
  QApplication * app;
  int myargc;
  char ** myargv;

  bool useWireframe;


  void run();

  QtApplication(int & argc, char ** argv);
  ~QtApplication();

  virtual void renderFrame() = 0;
  virtual void scene() = 0;

  void setWireframe(bool wire);

public slots:
  void draw();
  void setLazy(bool lazy);
};
