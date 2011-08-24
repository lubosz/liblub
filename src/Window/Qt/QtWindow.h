/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "Window/Window.h"

class QtWindow : public LL::Window {

 private:
	void toggleFullScreen();
	void toggleMouseGrab();
	void mouseLook(int x, int y);

 public:
	QtWindow();
	~QtWindow();
	void init(string title);
	void swapBuffers();
	void updateWindowTitle();
};
