
/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "QtWindow.h"
#include "QtInput.h"
#include "System/Logger.h"
#include "System/Timer.h"

QtWindow::QtWindow() {
  LogError << "Qt backend for MediaLayer is stub";
  fullscreen = false;
  grab = false;
  quit = false;
}

void QtWindow::init(string title) {
  programTile = title;
}

QtWindow::~QtWindow() {
}

void QtWindow::mouseLook(int x, int y) {
}

void QtWindow::swapBuffers(){
}

void QtWindow::toggleFullScreen(){
}

void QtWindow::toggleMouseGrab(){
}

void QtWindow::updateWindowTitle(){
}
