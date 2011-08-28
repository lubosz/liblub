/*
 * Copyright © 2010 Lubosz Sarnecki
 * Application.h
 *
 *  Created on: Jul 20, 2010
 */

#pragma once
#include <QApplication>

#ifdef WITH_SFML
#include "Window/SFML/SFMLWindow.h"
#endif

#ifdef WITH_Qt
#include "Window/Qt/QtWindow.h"
#endif

#ifdef WITH_XCB
#include "Window/XCB/XCBWindow.h"
#endif

#ifdef WITH_SDL
#include "Window/SDL/SDLWindow.h"
#endif

#include "Material/Materials.h"
#include "Mesh/MeshLoader.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "System/GUI.h"

enum WindowType { windowXCB, windowQt, windowSFML, windowSDL };

class Application : public QObject{

  Q_OBJECT

 public:
  GUI * gui;
	LL::Window * window;
  int argcount;
  char ** argvalues;
	QApplication * app;

  Application(int argc, char ** argv);
	virtual ~Application() {}
	virtual void scene() = 0;
	virtual void renderFrame() = 0;

	void chooseMediaLayer(WindowType type);
	void run();
	void run(WindowType type);
	void setFontOverlay(bool fontOverlay);

public slots:
	void updateFont();
	void draw();
	void eventLoop();

protected:
	bool fontOverlay;
};

