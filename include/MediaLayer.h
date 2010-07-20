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

private:
	Uint32 fps_lasttime; //the last recorded time.
	Uint32 fps_current; //the current FPS.
	Uint32 fps_frames; //frames passed since the last recorded fps.

	SDL_WindowID mainWindow;
	SDL_GLContext mainContext; /* Our opengl context handle */
	SDL_Event event;

	string programTile;

	bool quit;

	void eventLoop();
	void sdlDie(string msg);

public:
	MediaLayer(string title, unsigned width, unsigned height);
	virtual ~MediaLayer();

	void swapBuffers();
	void renderLoop();

};
