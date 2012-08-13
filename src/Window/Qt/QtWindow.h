/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include <QWidget>
#include "Window/Window.h"

class QHBoxLayout;

class QtWindow: public QWidget {
  Q_OBJECT

public:
  QtWindow();
  ~QtWindow();
  QHBoxLayout *mainLayout;
protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);
private:
  void toggleFullScreen();
  void toggleMouseGrab();
  void mouseLook(int x, int y);
};
