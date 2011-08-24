/*
 * Copyright © 2010 Lubosz Sarnecki
 * Input.h
 *
 *  Created on: Sep 28, 2010
 */

#pragma once

#define SDL_NO_COMPAT
#include "SDL.h"
#include "SDL_compat.h"
#include "Window/Input.h"

class SDLWindow;

class SDLInput: Input {
public:

  SDLInput(SDLWindow * mediaLayer);
  virtual ~SDLInput();
  void eventLoop();
private:

  Uint8 *keystate;
  SDLWindow * mediaLayer;
  SDL_Event event;

};
