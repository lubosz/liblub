/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include <string>
#include "common/Singleton.h"
#include "Input.h"

using std::string;

class MediaLayer : public Singleton<MediaLayer> {
public:
	Input * input;
  bool quit;

  friend class Singleton<MediaLayer>;
	friend class Input;

	bool fullscreen;
	bool grab;


	string programTile;

	void toggleFullScreen(){};
	void toggleMouseGrab(){};
	void mouseLook(int x, int y){};

	MediaLayer(){};
	~MediaLayer(){};

 public:
	unsigned width, height;
	void init(string title){};
	void swapBuffers(){};
	void renderFrame(){};

 protected:
	void shutdown() {
		quit = true;
	}
};
