/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#define SDL_NO_COMPAT
#include "SDL.h"

#include "Window/MediaLayer.h"
#include "Renderer/RenderEngine.h"
#include "Window/Input.h"

class SDLMediaLayer: public MediaLayer {

private:

  Uint32 fps_lasttime; // the last recorded time.
  Uint32 fps_current; // the current FPS.
  Uint32 fps_frames; // frames passed since the last recorded fps.

  SDL_Window *mainWindow;
  SDL_GLContext mainContext; /* Our opengl context handle */
  SDL_DisplayMode fsmode;

public:
  SDLMediaLayer();
  ~SDLMediaLayer();
  void toggleFullScreen();
  void toggleMouseGrab();
  void mouseLook(int x, int y);
  void init(string title);
  void swapBuffers();
  void renderFrame();
};
