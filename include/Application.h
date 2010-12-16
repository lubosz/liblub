/*
 * Application.h
 *
 *  Created on: Jul 20, 2010
 *      Author: bmonkey
 */

#pragma once

#include "MediaLayer.h"
#include "Materials.h"
#include "MeshFactory.h"
#include "SceneLoader.h"

class Application {

	virtual void scene() = 0;

public:

	string programName;
	SceneLoader * sceneLoader;

	void run(){
		MediaLayer::Instance().init(programName);
		scene();
		MediaLayer::Instance().renderLoop();

	}

};

