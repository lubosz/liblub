/*
 * PlanetsApp.h
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#pragma once

#include "common/OpenGL.h"
#include <QApplication>
#include <QtGui>
#include "Window/Qt/QtWindow.h"
#include "Window/Qt/glWidget.h"
#include "System/GUI.h"


class QtApplication: public QApplication {

Q_OBJECT

public:
  GLWidget *glWidget;
  GUI* gui;
  QtWindow * window;
  QTimer *drawTimer;
  bool fontOverlay;

  void run();

  QtApplication(int & argc, char ** argv);
  ~QtApplication();

  virtual void renderFrame() = 0;
  virtual void scene() = 0;
  virtual void initWidgets(QHBoxLayout * mainLayout) = 0;

public slots:
  void draw();
  void setLazy(bool lazy);
};
