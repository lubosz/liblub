/*
 * Copyright © 2010 Lubosz Sarnecki
 * Application.h
 *
 *  Created on: Jul 20, 2010
 */

#pragma once

#include "MediaLayer.h"
#include "Materials.h"
#include "MeshFactory.h"
#include "SceneLoader.h"
#include "Config.h"

class Application {
 public:
	virtual ~Application() {}
	virtual void scene() = 0;

	string programName;
	SceneLoader * sceneLoader;

	void run() {
		Config::Instance().load("config.xml");
		MediaLayer::Instance().init(programName);
		scene();
		MediaLayer::Instance().renderLoop();
	}
};

