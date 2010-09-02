/*
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

//#define SDL_NO_COMPAT

#include "common.h"
#include <SDL/SDL.h>
//#include <SDL/SDL_opengl.h>
#include <string>
#include "RenderEngine.h"




class MediaLayer {

private:
	Uint32 fps_lasttime; //the last recorded time.
	Uint32 fps_current; //the current FPS.
	Uint32 fps_frames; //frames passed since the last recorded fps.

	SDL_WindowID mainWindow;
	SDL_GLContext mainContext; /* Our opengl context handle */
    SDL_DisplayMode fsmode;
	SDL_Event event;

	bool fullscreen;

	string programTile;

	bool quit;

	void eventLoop();
	void sdlDie(string msg);
	void toggleFullScreen();

public:
	MediaLayer(string title, unsigned width, unsigned height);
	virtual ~MediaLayer();

	void swapBuffers();
	void renderLoop();

};
