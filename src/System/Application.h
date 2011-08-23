/*
 * Copyright © 2010 Lubosz Sarnecki
 * Application.h
 *
 *  Created on: Jul 20, 2010
 */

#pragma once
#include <QApplication>

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

class Application : public QApplication {

  Q_OBJECT

 public:
  GUI * gui;
	SceneLoader * sceneLoader;
	MediaLayer * mediaLayer;
	int argc;

  Application() :QApplication(argc,0){ }
	virtual ~Application() {}
	virtual void scene() = 0;
	virtual void renderFrame() = 0;

	void chooseMediaLayer(MediaLayerType type);
	void run();
	void run(MediaLayerType type);
	void setFontOverlay(bool fontOverlay);
public slots:
	void updateFont();
	void draw();
	void eventLoop();

protected:
	bool fontOverlay;
};

