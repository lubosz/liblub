/*
 * Copyright © 2010 Lubosz Sarnecki
 * Application.h
 *
 *  Created on: Jul 20, 2010
 */

#pragma once

#include "Window/MediaLayer.h"
#include "Material/Materials.h"
#include "Mesh/MeshFactory.h"
#include "Scene/SceneLoader.h"
#include "System/Config.h"

class Application {
 public:
	virtual ~Application() {}
	virtual void scene() = 0;

	string programName;
	SceneLoader * sceneLoader;

	void run() {
		Config::Instance().load("config.xml");
		MediaLayer::Instance();
		scene();
		MediaLayer::Instance().init(programName);
		MediaLayer::Instance().renderLoop();
	}
};

