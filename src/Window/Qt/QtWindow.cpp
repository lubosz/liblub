
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
#include "Scene/SceneData.h"

QtWindow::QtWindow() {
  LogError << "Qt backend for MediaLayer is stub";
  fullscreen = false;
  grab = false;
  quit = false;
  input = new QtInput();
  mainLayout = new QHBoxLayout;
  setLayout(mainLayout);
}

void QtWindow::init(string title) {
  programTile = title;
}

QtWindow::~QtWindow() {
}

void QtWindow::mouseLook(int x, int y) {
  LogDebug << x << y;
}

void QtWindow::swapBuffers(){
}

void QtWindow::toggleFullScreen(){
}

void QtWindow::toggleMouseGrab(){
}

void QtWindow::updateWindowTitle(){
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
        SceneData::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
      if (key == Qt::Key_A)
        SceneData::Instance().getCurrentCamera()->leftDirection(inputSpeed);
      if (key == Qt::Key_S)
        SceneData::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
      if (key == Qt::Key_D)
        SceneData::Instance().getCurrentCamera()->rightDirection(inputSpeed);
  }
  emit draw();
}

void QtWindow::keyReleaseEvent(QKeyEvent *e) {
  pressedKeys.removeAll(e->key());
}
