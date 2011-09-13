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
signals:
  void draw();
protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);
  void executeKeys();
private:
  QList<int> pressedKeys;
  void toggleFullScreen();
  void toggleMouseGrab();
  void mouseLook(int x, int y);
};
