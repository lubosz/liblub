/*
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"

#ifdef WITH_SDL
	#define SDL_NO_COMPAT
	#include "SDL.h"
#else
	#include <X11/Xlib.h>
	#include <X11/Xlib-xcb.h>

	#include <xcb/xcb.h>

#endif
#include <sstream>

#include "RenderEngine.h"
#include "Input.h"

class MediaLayer : public Singleton<MediaLayer>{

private:

#ifdef WITH_SDL
	Uint32 fps_lasttime; //the last recorded time.
	Uint32 fps_current; //the current FPS.
	Uint32 fps_frames; //frames passed since the last recorded fps.

	SDL_Window *mainWindow;
	SDL_GLContext mainContext; /* Our opengl context handle */
    SDL_DisplayMode fsmode;
	SDL_Event event;
	Uint8 *keystate;
#else
	unsigned fps_lasttime; //the last recorded time.
	unsigned fps_current; //the current FPS.
	unsigned fps_frames; //frames passed since the last recorded fps.

    int visualID;
    int default_screen;

    Display *display;

    xcb_screen_t *screen;
    xcb_connection_t *connection;
    xcb_window_t window;
    xcb_colormap_t colormap;
    xcb_cursor_t cursor; /* blank cursor */

    GLXWindow glxwindow;
    GLXContext context;
    GLXDrawable drawable;
    GLXFBConfig fb_config;

    //Input
    Input * input;
    uint32_t eventmask;

    void setWindowTitle(string title);
    void initScreen();
    void createGLContext();
    void createWindow();
    void initFrameBuffer();
    void createColorMap();
    void createBlankCursor();

#endif

    friend class Singleton<MediaLayer>;
	friend class Input;

	bool fullscreen;
	bool grab;
	bool quit;

	unsigned width, height;

	string programTile;

	void toggleFullScreen();
	void getFPS();
	void toggleMouseGrab();
	void mouseLook(int x, int y);

	MediaLayer();
	~MediaLayer();

public:
	void init(string title, unsigned width, unsigned height);
	void swapBuffers();
	void renderLoop();

protected:
	void error(string msg);
	void shutdown(){
		quit = true;
	}
};
