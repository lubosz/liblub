/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

// Include first OpenGL then Qt then Xlib / GLX!
#include "Renderer/OpenGL.h"

// Needed for GL3 Context Creation
//#define GLX_GLXEXT_PROTOTYPES 1
// Patched glx with gl3
//#include <GL3/glx.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

#include <xcb/xcb.h>

#include "System/Timer.h"
#include "Application/Window.h"
class XCBWindow: public LL::Window {

  friend class XCBInput;

private:
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

  Timer * timer;

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

  void toggleFullScreen();
  void toggleMouseGrab();
  void mouseLook(int x, int y);

public:
  XCBWindow();
  virtual ~XCBWindow();
  void updateWindowTitle();
  void init(string title);
  void swapBuffers();
  void update();
};
