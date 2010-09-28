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
	#include <xcb/xcb_keysyms.h>
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

    Display *display;
    xcb_screen_t *screen;
    int visualID;
    int default_screen;
    GLXWindow glxwindow;
    xcb_connection_t *connection;
    xcb_window_t window;
    GLXContext context;
    GLXDrawable drawable;
    GLXFBConfig fb_config;
    xcb_colormap_t colormap;

    //Input
    Input * input;


    uint32_t eventmask;

    int mouseLastX, mouseLastY;

    void setWindowTitle(string title);
    void initScreen();
    void createGLContext();
    void createWindow();
    void initFrameBuffer();
    void createColorMap();
    void setupEventHandlers();
    void getKey(xcb_keycode_t key);
    void setupXi2();
    void xi2Event();
    void xcbEventHandlers();
    void xcbEventLoop();

#endif

  friend class Singleton<MediaLayer>;
	friend class Input;

	bool fullscreen;
	bool grab;

	unsigned width, height;

	string programTile;

	bool quit;

	void eventLoop();
	void eventLoop2();

	void toggleFullScreen();
	void getFPS();


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
