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
#include <QDomElement>

class Application {

	virtual void scene() = 0;

public:

	string programName;
	QDomElement sceneXML;

	void run(){
		MediaLayer::Instance().init(programName);
		scene();
		MediaLayer::Instance().renderLoop();

	}

};

