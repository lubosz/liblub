/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "Window/Input.h"
#include "Scene/SceneData.h"
#include "Window/MediaLayer.h"
#include "Renderer/RenderEngine.h"
#include "System/Logger.h"

Input::Input() {
  // TODO(bmonkey): Hardcoded values => xml
  inputSpeed = .01;
  mouseSensitivity = .1;
}

Input::~Input() {
	// TODO Auto-generated destructor stub
}

void Input::eventLoop(){
}
