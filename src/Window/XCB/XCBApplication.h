/*
 * Copyright © 2010 Lubosz Sarnecki
 * XCBApplication.h
 *
 *  Created on: Jul 20, 2010
 */

#pragma once
#include <QApplication>
#include "Window/XCB/XCBWindow.h"
#include "Mesh/MeshLoader.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "System/GUI.h"

class XCBApplication : public QObject{

  Q_OBJECT

 public:
  GUI * gui;
	XCBWindow * window;
  int argcount;
  char ** argvalues;
	QApplication * app;

  XCBApplication(int argc, char ** argv);
	virtual ~XCBApplication() {}
	virtual void scene() = 0;
	virtual void renderFrame() = 0;

	void run();
	void setFontOverlay(bool fontOverlay);

public slots:
	void updateFont();
	void draw();

protected:
	bool fontOverlay;
};

