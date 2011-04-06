/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 */
#include <sstream>
#include <string>
#include <vector>
#include "Window/MediaLayer.h"
#include "Scene/Camera.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "System/Logger.h"
#include "System/GUI.h"

MediaLayer::MediaLayer() {
  // FPS Stuff
  fps_lasttime = 0;  // the last recorded time.
  fps_frames = 0;  // frames passed since the last recorded fps.
  input_lasttime = 0;

  fullscreen = false;
  grab = false;
  quit = false;
  initScreen();
  initFrameBuffer();
  createGLContext();
  createColorMap();

  createWindow();
  createBlankCursor();

}

void MediaLayer::init(string title) {
  programTile = title;


  input = new Input(connection);

  // Toggle mouse at init
  unsigned halfWidth = width/2;
  unsigned halfHeight = height/2;
  toggleMouseGrab();
  if (grab)
      XWarpPointer(
              display, None, window, 0, 0,
              width, height, halfWidth, halfHeight);

  SceneData::Instance().getCurrentCamera()->setAspect(
          static_cast<float>(MediaLayer::Instance().width)/
          static_cast<float>(MediaLayer::Instance().height));
}

MediaLayer::~MediaLayer() {
    /* Cleanup */
    glXDestroyWindow(display, glxwindow);
    xcb_destroy_window(connection, window);
    glXDestroyContext(display, context);
    XCloseDisplay(display);
}

void MediaLayer::createBlankCursor() {
  cursor = xcb_generate_id(connection);
  xcb_pixmap_t pix = xcb_generate_id(connection);

  xcb_create_pixmap(connection, 1, pix, screen->root, 1, 1);
  xcb_create_cursor(connection, cursor, pix, pix, 0, 0, 0, 0, 0, 0, 1, 1);
}

void MediaLayer::initScreen() {
    /* Open Xlib Display */
    display = XOpenDisplay(0);
    if (!display) Logger::Instance().log("ERROR",
            "initScreen", "Can't open display");

    default_screen = DefaultScreen(display);

    /* Get the XCB connection from the display */
    connection = XGetXCBConnection(display);
    if (!connection) Logger::Instance().log("ERROR",
            "initScreen", "Can't get xcb connection from display");

    /* Acquire event queue ownership */
    XSetEventQueueOwner(display, XCBOwnsEventQueue);

    /* Find XCB screen */
    screen = 0;
    xcb_screen_iterator_t screen_iter =
            xcb_setup_roots_iterator(xcb_get_setup(connection));
    for (int screen_num = default_screen;
        screen_iter.rem && screen_num > 0;
        --screen_num, xcb_screen_next(&screen_iter));
    screen = screen_iter.data;

  width = screen->width_in_pixels;
  height = screen->height_in_pixels;
}

void MediaLayer::initFrameBuffer() {
    /* Query framebuffer configurations */
    GLXFBConfig *fb_configs = 0;
    int num_fb_configs = 0;
    fb_configs = glXGetFBConfigs(display, default_screen, &num_fb_configs);
    if (!fb_configs || num_fb_configs == 0)
        Logger::Instance().log("ERROR",
                "initFrameBuffer", "glXGetFBConfigs failed");
    fb_config = fb_configs[0];
}

void MediaLayer::createGLContext() {
  /* Initialize window and OpenGL context */
  visualID = 0;

  /* Select first framebuffer config and query visualID */
  glXGetFBConfigAttrib(display, fb_config, GLX_VISUAL_ID, &visualID);

  vector<int> glContext = Config::Instance().values<int>("GLcontext");

  int attribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB,
      glContext[0],
      GLX_CONTEXT_MINOR_VERSION_ARB,
      glContext[1],
      0
  };

  /* Get a pointer to the context creation function for GL 3.0 */
  PFNGLXCREATECONTEXTATTRIBSARBPROC
      glXCreateContextAttribs =
          (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress(
              reinterpret_cast<GLubyte const *>("glXCreateContextAttribsARB"));
  if (!glXCreateContextAttribs) {
    printf("GL 3.x is not supported");
  }

  context = glXCreateContextAttribs(display, fb_config, NULL, True,
      attribs);

  if (!context)
      Logger::Instance().log("ERROR",
              "createGLContext", "glXCreateNewContext failed");
}

void MediaLayer::createColorMap() {
  /* Create XID's for colormap and window */
  colormap = xcb_generate_id(connection);
  window = xcb_generate_id(connection);

  /* Create colormap */
  xcb_create_colormap(
      connection,
      XCB_COLORMAP_ALLOC_NONE,
      colormap,
      screen->root,
      visualID);
}

void MediaLayer::createWindow() {
  eventmask =
      XCB_EVENT_MASK_KEY_PRESS |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_KEY_RELEASE;
  uint32_t valuelist[] = { eventmask, colormap };
  uint32_t valuemask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

  xcb_create_window(
      connection,
      XCB_COPY_FROM_PARENT,
      window,
      screen->root,
      0, 0,
      width, height,
      0,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      visualID,
      valuemask,
      valuelist);


  // NOTE: window must be mapped before glXMakeContextCurrent
  xcb_map_window(connection, window);

  /* Create GLX Window */
  glxwindow = glXCreateWindow(display, fb_config, window, 0);

  if (!glxwindow)
    Logger::Instance().log("ERROR",
            "createWindow", "glXCreateWindow failed");

  drawable = glxwindow;

  Logger::Instance().log("MESSAGE", "XCB", "Making Context Current");
  /* make OpenGL context current */
  if (!glXMakeContextCurrent(display, drawable, drawable, context))
    Logger::Instance().log("ERROR",
            "createWindow", "glXMakeContextCurrent failed");
  Logger::Instance().log("MESSAGE", "XCB", "Context activated");

  // Set swap interval
  PFNGLXSWAPINTERVALSGIPROC
    glXSwapInterval =
          (PFNGLXSWAPINTERVALSGIPROC) glXGetProcAddress(
                  reinterpret_cast<GLubyte const *>("glXSwapIntervalSGI"));
  if (!glXSwapInterval) {
    Logger::Instance().log("MediaLayer", "WARNING", "VSync is not supported");
  } else {
    glXSwapInterval(Config::Instance().value<int>("Vsync"));
  }
  setWindowTitle(programTile);
}

void MediaLayer::swapBuffers() {
  glXSwapBuffers(display, drawable);
}
/*
static void
  wmspec_change_state (bool   add,
      xcb_intern_atom_cookie_t state1,
      xcb_intern_atom_cookie_t state2)
{
  GdkDisplay *display = GDK_WINDOW_DISPLAY (window);
  XClientMessageEvent xclient;

#define _NET_WM_STATE_REMOVE        0    // remove/unset property
#define _NET_WM_STATE_ADD           1    // add/set property
#define _NET_WM_STATE_TOGGLE        2    // toggle property


  xcb_client_message_event_t ev;
    memset (&ev, 0, sizeof (ev));
  ev.response_type = XCB_CLIENT_MESSAGE;
  ev.window = window;
  ev.type = wm_state;
  ev.format = 32;
  ev.data.data32[0] = 2; //toggle
  ev.data.data32[1] = wm_state_fullscreen;
  ev.data.data32[2] = wm_state_fullscreen;
  ev.data.data32[3] = 0;
  ev.data.data32[4] = 0;

  memset (&xclient, 0, sizeof (xclient));
  xclient.type = ClientMessage;
  xclient.window = GDK_WINDOW_XID (window);
  xclient.message_type = gdk_x11_get_xatom_by_name_for_display (display, "_NET_WM_STATE");
  xclient.format = 32;
  xclient.data.l[0] = add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
  xclient.data.l[1] = gdk_x11_atom_to_xatom_for_display (display, state1);
  xclient.data.l[2] = gdk_x11_atom_to_xatom_for_display (display, state2);
  xclient.data.l[3] = 0;
  xclient.data.l[4] = 0;

  xcb_send_event(
      connection,
      0,
      window,
      XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
      (const char *) &ev
  );

}
*/
xcb_intern_atom_cookie_t MediaLayer::getCookieForAtom(string state_name) {
  return xcb_intern_atom(
    connection, 0, state_name.length(), state_name.c_str());
}

xcb_atom_t MediaLayer::getReplyAtomFromCookie(xcb_intern_atom_cookie_t cookie) {
  xcb_generic_error_t * error;
  xcb_intern_atom_reply_t *reply =
          xcb_intern_atom_reply(connection, cookie, &error);
  if (error) {
      Logger::Instance().message
              << "Can't set the screen. Error Code: "<< error->error_code;
      Logger::Instance().log("XCB", "ERROR", "");
  }
  return reply->atom;
}

void MediaLayer::toggleFullScreen() {
  if (fullscreen) {
    Logger::Instance().log("MESSAGE", "Fullscreen", "off");
  } else {
    Logger::Instance().log("MESSAGE", "Fullscreen", "on");
  }
  fullscreen = !fullscreen;

  xcb_intern_atom_cookie_t wm_state_ck =
          getCookieForAtom("_NET_WM_STATE");
  xcb_intern_atom_cookie_t wm_state_fs_ck =
          getCookieForAtom("_NET_WM_STATE_FULLSCREEN");

#define _NET_WM_STATE_REMOVE        0    // remove/unset property
#define _NET_WM_STATE_ADD           1    // add/set property
#define _NET_WM_STATE_TOGGLE        2    // toggle property

  xcb_client_message_event_t ev;
  // memset (&ev, 0, sizeof (ev));
  ev.response_type = XCB_CLIENT_MESSAGE;
  ev.type = getReplyAtomFromCookie(wm_state_ck);
  ev.format = 32;
  ev.window = window;
  // ev.data.data32[0] = fullscreen ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
  ev.data.data32[0] = _NET_WM_STATE_TOGGLE;
    ev.data.data32[1] = getReplyAtomFromCookie(wm_state_fs_ck);
    ev.data.data32[2] = XCB_ATOM_NONE;
    ev.data.data32[3] = 0;
    ev.data.data32[4] = 0;

  xcb_send_event(
      connection,
      1,
      window,
      XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
      (const char *) &ev);
}

void MediaLayer::renderLoop() {
    while (!quit) {
//      input->eventLoop();
      RenderEngine::Instance().display();
      swapBuffers();
      getFPS();
     }
}

void MediaLayer::getFPS() {
  struct timespec now;
  static struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &now);
  unsigned ticks = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec
      - start.tv_nsec) / 1000000;
  fps_frames++;

  //check input every 1/100 secound
    if (input_lasttime < ticks - 10) {
      input->eventLoop();
      input_lasttime = ticks;
    }

   //reset fps counter every secound
  if (fps_lasttime < ticks - 1000) {
    fps_lasttime = ticks;
    fps_current = fps_frames;
    fps_frames = 0;
    GUI::Instance().update();
  }
    // TODO(bmonkey): should be per secound, and not per frame (breaks mouse input)
    stringstream windowTitle;
//    windowTitle << programTile << " - FPS: " << fps_current;
    windowTitle << "FPS: " << fps_current;
    setWindowTitle(windowTitle.str());

}

void MediaLayer::setWindowTitle(string title) {
    xcb_change_property(
      connection,
      XCB_PROP_MODE_REPLACE,
      window,
    XCB_ATOM_WM_NAME,
    XCB_ATOM_STRING,
    8,
    title.length(),
    title.c_str());
}

void MediaLayer::toggleMouseGrab() {
  if (!grab) {
    // hide cursor
    uint32_t value_list = cursor;

        xcb_change_window_attributes(
            connection,
            window,
                XCB_CW_CURSOR,
                &value_list);
    grab = true;
  } else {
      /* show the default cursor */
    uint32_t value_list = XCB_CURSOR_NONE;
      xcb_change_window_attributes(
          connection,
          window,
          XCB_CW_CURSOR,
          &value_list);
    grab = false;
  }
}

void MediaLayer::mouseLook(int x, int y) {
  unsigned halfWidth = width/2;
  unsigned halfHeight = height/2;

  int xRel = x - halfWidth;
  int yRel = y - halfHeight;

  if (!(xRel == 0 && yRel == 0) && grab) {
    SceneData::Instance().getCurrentCamera()->setMouseLook(xRel, yRel, input->mouseSensitivity);
    if (grab)
        XWarpPointer(
                display, None, window, x, y,
                width, height, halfWidth, halfHeight);
  }
}
