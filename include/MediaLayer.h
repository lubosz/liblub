/*
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"

#ifdef USE_SDL
	#define SDL_NO_COMPAT
	#include <SDL/SDL.h>
#else
	#include <X11/Xlib.h>
	#include <X11/Xlib-xcb.h>
	#include <xcb/xcb.h>
#endif
#include <sstream>

#include "RenderEngine.h"

class MediaLayer {

private:

#ifdef USE_SDL
	Uint32 fps_lasttime; //the last recorded time.
	Uint32 fps_current; //the current FPS.
	Uint32 fps_frames; //frames passed since the last recorded fps.

	SDL_Window *mainWindow;
	SDL_GLContext mainContext; /* Our opengl context handle */
    SDL_DisplayMode fsmode;
	SDL_Event event;
#else
	unsigned fps_lasttime; //the last recorded time.
	unsigned fps_current; //the current FPS.
	unsigned fps_frames; //frames passed since the last recorded fps.

    Display *display;
    int default_screen;
    GLXWindow glxwindow;
    xcb_connection_t *connection;
    xcb_window_t window;
    GLXContext context;
    GLXDrawable drawable;

    //Input
    xcb_generic_event_t *event;
    xcb_expose_event_t *expose;
    xcb_button_press_event_t *bp;
    xcb_button_release_event_t *br;
    xcb_motion_notify_event_t *motion;
    xcb_enter_notify_event_t *enter;
	xcb_leave_notify_event_t *leave;
	xcb_key_press_event_t *kp;
	xcb_key_release_event_t *kr;

	int relX, relY;
    int mouseLastX, mouseLastY;

    void setWindowTitle(string title);

#endif
	bool fullscreen;
	bool grab;

	string programTile;

	bool quit;

	void eventLoop();
	void error(string msg);
	void toggleFullScreen();
	void getFPS();

public:
	MediaLayer(string title, unsigned width, unsigned height);
	virtual ~MediaLayer();

	void swapBuffers();
	void renderLoop();

};
