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
	#include <xcb/xcb_event.h>
	#include <xcb/xcb.h>
	#include <xcb/xcb_keysyms.h>
#endif
#include <sstream>

#include "RenderEngine.h"

class MediaLayer {

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
    int opcode;

    uint32_t eventmask;


    xcb_event_handlers_t *evenths;


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
    void initScreen();
    void createGLContext();
    void createWindow();
    void initFrameBuffer();
    void createColorMap();
    void setupEventHandlers();
    void getKey(xcb_keycode_t key);

#define HANDLER_DECLARE( event, name )\
  static int Handle##name( void*, xcb_connection_t*, xcb_##event##_event_t* )

  HANDLER_DECLARE( motion_notify, MotionNotify );
  HANDLER_DECLARE( key_press, KeyPress );

#undef HANDLER_DECLARE

    //int HandleMotionNotify( void *data, xcb_connection_t * __UNUSED__, xcb_motion_notify_event_t *event );

#endif
	bool fullscreen;
	bool grab;

	unsigned width, height;

	string programTile;

	bool quit;

	void eventLoop();
	void eventLoop2();
	void error(string msg);
	void toggleFullScreen();
	void getFPS();

public:
	MediaLayer(string title, unsigned width, unsigned height);
	virtual ~MediaLayer();

	void swapBuffers();
	void renderLoop();

};
