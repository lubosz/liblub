
/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include <QtGui>
#include "QtWindow.h"
#include "QtInput.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "Scene/Scene.h"

QtWindow::QtWindow() {
  mainLayout = new QHBoxLayout;
  setLayout(mainLayout);
}

QtWindow::~QtWindow() {
}

void QtWindow::mouseLook(int x, int y) {
  LogDebug << x << y;
}

void QtWindow::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Escape)
    close();
  else
    pressedKeys.push_back(e->key());

  executeKeys();
}

void QtWindow::executeKeys() {
  float inputSpeed = .1;
  foreach(int key, pressedKeys) {
      if (key == Qt::Key_W)
        Scene::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
      if (key == Qt::Key_A)
        Scene::Instance().getCurrentCamera()->leftDirection(inputSpeed);
      if (key == Qt::Key_S)
        Scene::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
      if (key == Qt::Key_D)
        Scene::Instance().getCurrentCamera()->rightDirection(inputSpeed);
  }
  emit draw();
}

void QtWindow::keyReleaseEvent(QKeyEvent *e) {
  pressedKeys.removeAll(e->key());
}
