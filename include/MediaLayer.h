/*
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once



//Include first OpenGL then Qt then Xlib / GLX!
#include "RenderEngine.h"
#include "Input.h"
//#include <QtCore>
//#include "Qt3D.h"
//#include "common.h"

#ifdef WITH_SDL
	#define SDL_NO_COMPAT
	#include "SDL.h"
#else
	#define GLX_GLXEXT_PROTOTYPES 1 //Needed for GL3 Context Creation
	#include <GL3/glx.h>

	#include <X11/Xlib.h>
	#include <X11/Xlib-xcb.h>

	#include <xcb/xcb.h>

#endif


class MediaLayer : public Singleton<MediaLayer>{

private:
    Input * input;

#ifdef WITH_SDL
	Uint32 fps_lasttime; //the last recorded time.
	Uint32 fps_current; //the current FPS.
	Uint32 fps_frames; //frames passed since the last recorded fps.

	SDL_Window *mainWindow;
	SDL_GLContext mainContext; /* Our opengl context handle */
    SDL_DisplayMode fsmode;


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

    uint32_t eventmask;

    void setWindowTitle(string title);
    void initScreen();
    void createGLContext();
    void createWindow();
    void initFrameBuffer();
    void createColorMap();
    void createBlankCursor();

    //Fullscreen
    xcb_atom_t wm_state, wm_state_fullscreen;

#endif

    friend class Singleton<MediaLayer>;
	friend class Input;

	bool fullscreen;
	bool grab;
	bool quit;



	string programTile;

	void toggleFullScreen();
	void getFPS();
	void toggleMouseGrab();
	void mouseLook(int x, int y);

	MediaLayer();
	~MediaLayer();

public:
	unsigned width, height;
	void init(string title);
	void swapBuffers();
	void renderLoop();

protected:
	void error(string msg);
	void shutdown(){
		quit = true;
	}
};
