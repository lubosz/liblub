/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "Window/MediaLayer.h"

class QtMediaLayer : public MediaLayer {

 private:
	void toggleFullScreen();
	void toggleMouseGrab();
	void mouseLook(int x, int y);

 public:
	QtMediaLayer();
	~QtMediaLayer();
	void init(string title);
	void swapBuffers();
	void updateWindowTitle();
};
