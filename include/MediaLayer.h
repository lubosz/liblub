/*
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include <SDL/SDL.h>
//#include <SDL/SDL_opengl.h>

#include <string>

#include "RenderEngine.h"

using namespace std;

class MediaLayer {
public:

	SDL_WindowID mainWindow;
	SDL_GLContext mainContext; /* Our opengl context handle */
	SDL_Event event;

	bool quit;

	MediaLayer(string title, unsigned width, unsigned height);
	virtual ~MediaLayer();

	void sdlDie(string msg);

	void swapBuffers();
	void renderLoop();
	void eventLoop();

};
