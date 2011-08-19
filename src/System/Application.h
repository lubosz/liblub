/*
 * Copyright © 2010 Lubosz Sarnecki
 * Application.h
 *
 *  Created on: Jul 20, 2010
 */

#pragma once

#ifdef WITH_SFML
#include "Window/SFML/SFMLMediaLayer.h"
#endif

#ifdef WITH_Qt
#include "Window/Qt/QtMediaLayer.h"
#endif

#ifdef WITH_XCB
#include "Window/XCB/XCBMediaLayer.h"
#endif

#ifdef WITH_SDL
#include "Window/SDL/SDLMediaLayer.h"
#endif

#include "Material/Materials.h"
#include "Mesh/MeshLoader.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "System/GUI.h"


enum MediaLayerType { windowXCB, windowQt, windowSFML, windowSDL };

class Application {
 public:
	virtual ~Application() {}
	virtual void scene() = 0;
	virtual void renderFrame() = 0;

	SceneLoader * sceneLoader;
	MediaLayer * mediaLayer;

	void chooseMediaLayer(MediaLayerType type) {
	  if (type == windowXCB) {
#ifdef WITH_XCB
	    mediaLayer = new XCBMediaLayer();
	    LogInfo << "Using XCB for Input and Window";
#endif
	  } else if (type == windowQt) {
#ifdef WITH_Qt
	    mediaLayer = new QtMediaLayer();
	    LogInfo << "Using Qt for Input and Window";
#endif
	  } else if (type == windowSFML) {
#ifdef WITH_SFML
	    mediaLayer = new SFMLMediaLayer();
	    LogInfo << "Using SFML for Input and Window";
#endif
	  } else if (type == windowSDL) {
#ifdef WITH_SDL
	    mediaLayer = new SDLMediaLayer();
	    LogInfo << "Using SDL for Input and Window";
#endif
	  }
	}

	void run() {
#if WITH_XCB
	  run(windowXCB);
#elif WITH_SDL
    run(windowSDL);
#elif WITH_SFML
    run(windowSFML);
//#elif WITH_Qt
//    run(windowQt);
#else
    LogError << "No Media Layer compiled.";
#endif
	}

	void run(MediaLayerType type) {
		Config::Instance().load("config.xml");
	  chooseMediaLayer(type);
	  string sceneName = SceneData::Instance().name;
	  LogDebug << "Initializing Media Layer";
		mediaLayer->init(sceneName);
		scene();
		GUI::Instance().init();
    while (!mediaLayer->quit) {
      mediaLayer->renderFrame();
      renderFrame();
     }
	}
};

