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

#include <SFML/Graphics.hpp>
#include "Window/MediaLayer.h"

class SFMLMediaLayer: public MediaLayer {
public:

  sf::Window * window;

public:
  void toggleFullScreen();
  void toggleMouseGrab();
  void mouseLook(int x, int y);

  SFMLMediaLayer();
  ~SFMLMediaLayer();
  void init(string title);
  void swapBuffers();
  void renderFrame();
};
