/*
 * Copyright © 2010 Lubosz Sarnecki
 * XCBApplication.h
 *
 *  Created on: Jul 20, 2010
 */

#pragma once
#include <QtCore>
#include <QApplication>
#include "Window/XCB/XCBWindow.h"
#include "Scene/Scene.h"
#include "System/Config.h"
#include "System/FontOverlay.h"

class XCBApplication : public QObject{

  Q_OBJECT

 public:
  FontOverlay * fontOverlay;
	XCBWindow * window;
  int argcount;
  char ** argvalues;
    QApplication * app;

  XCBApplication(int argc, char ** argv);
    virtual ~XCBApplication();
	virtual void scene() = 0;
	virtual void renderFrame() = 0;

	void run();

    void setFontOverlay(bool useFontOverlay);

public slots:
	void updateFont();
	void draw();

protected:
    bool useFontOverlay;
};

