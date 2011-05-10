/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

// Include first OpenGL then Qt then Xlib / GLX!
#include "Renderer/RenderEngine.h"
#include "Window/Input.h"

#if WITH_SDL
	#define SDL_NO_COMPAT
	#include "SDL.h"
#elif WITH_SFML
	#include <SFML/Graphics.hpp>
#elif WITH_XCB
    // Needed for GL3 Context Creation
	#define GLX_GLXEXT_PROTOTYPES 1
	#include <GL/glx.h>

	#include <X11/Xlib.h>
	#include <X11/Xlib-xcb.h>

	#include <xcb/xcb.h>
#endif


class MediaLayer : public Singleton<MediaLayer> {
public:
	Input * input;
  bool quit;
 private:

#ifdef WITH_SDL
	Uint32 fps_lasttime;  // the last recorded time.
	Uint32 fps_current;  // the current FPS.
	Uint32 fps_frames;  // frames passed since the last recorded fps.

	SDL_Window *mainWindow;
	SDL_GLContext mainContext; /* Our opengl context handle */
    SDL_DisplayMode fsmode;


#elif WITH_SFML
    sf::Window * window;
#elif WITH_XCB

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

    // Input

    uint32_t eventmask;

    void setWindowTitle(string title);
    void initScreen();
    void createGLContext();
    void createWindow();
    void initFrameBuffer();
    void createColorMap();
    void createBlankCursor();

    xcb_intern_atom_cookie_t getCookieForAtom(string state_name);
    xcb_atom_t getReplyAtomFromCookie(xcb_intern_atom_cookie_t cookie);
#endif

  friend class Singleton<MediaLayer>;
	friend class Input;

	bool fullscreen;
	bool grab;


	string programTile;

	void toggleFullScreen();
	void toggleMouseGrab();
	void mouseLook(int x, int y);

	MediaLayer();
	~MediaLayer();

 public:
	unsigned width, height;
	void init(string title);
	void swapBuffers();
	void renderFrame();

 protected:
	void shutdown() {
		quit = true;
	}
};
