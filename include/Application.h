/*
 * Application.h
 *
 *  Created on: Jul 20, 2010
 *      Author: bmonkey
 */

#pragma once

#include "MediaLayer.h"
#include "Camera.h"
#include "Materials.h"
#include "MeshFactory.h"

class Application {

	virtual void scene() = 0;

public:
	unsigned width;
	unsigned height;

	string programName;

	void run(){
		MediaLayer::Instance().init(programName, width, height);

		Camera::Instance().setParams(70.0, 0.1, 100.0);
		Camera::Instance().setAspect(float(width)/float(height));

		scene();

		MediaLayer::Instance().renderLoop();

	}

};

