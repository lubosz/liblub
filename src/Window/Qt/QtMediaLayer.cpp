
/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "QtMediaLayer.h"
#include "QtInput.h"
#include "System/Logger.h"
#include "System/Timer.h"

QtMediaLayer::QtMediaLayer() {
  LogError << "Qt backend for MediaLayer is stub";
  fullscreen = false;
  grab = false;
  quit = false;
}

void QtMediaLayer::init(string title) {
  programTile = title;
}

QtMediaLayer::~QtMediaLayer() {
}

void QtMediaLayer::mouseLook(int x, int y) {

}

void QtMediaLayer::swapBuffers(){
}

void QtMediaLayer::toggleFullScreen(){
}

void QtMediaLayer::toggleMouseGrab(){
}

void QtMediaLayer::renderFrame(){
  Timer::Instance().frame();
  swapBuffers();
}
