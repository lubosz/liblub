
/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */
#include <iostream>
#include <sstream>
#include "Window/MediaLayer.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "System/Config.h"
#include "System/Timer.h"

MediaLayer::MediaLayer() {
  LogError << "Qt backend for MediaLayer is stub";
  fullscreen = false;
  grab = false;
  quit = false;
  input = new Input();
}

void MediaLayer::init(string title) {
  programTile = title;
}

MediaLayer::~MediaLayer() {
}

void MediaLayer::swapBuffers(){
}

void MediaLayer::toggleFullScreen(){
}

void MediaLayer::toggleMouseGrab(){
}

void MediaLayer::renderFrame(){
  Timer::Instance().frame();
  swapBuffers();
}
