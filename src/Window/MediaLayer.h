/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include <string>
#include "Input.h"

using std::string;

class MediaLayer {
public:
	friend class Input;
	friend class XCBInput;
	friend class SDLInput;

	Input * input;
  bool quit;
	bool fullscreen;
	bool grab;
	string programTile;
	unsigned width, height;

	virtual void toggleFullScreen() = 0;
	virtual void toggleMouseGrab() = 0;
	virtual void mouseLook(int x, int y) = 0;
	virtual void init(string title) = 0;
	virtual void swapBuffers() = 0;
	virtual void updateWindowTitle() = 0;

 protected:
	void shutdown() {
		quit = true;
	}
};
