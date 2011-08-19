/*
 * Copyright © 2010 Lubosz Sarnecki
 * Input.h
 *
 *  Created on: Sep 28, 2010
 */

#pragma once

#include "Window/Input.h"

class SFMLMediaLayer;

class SFMLInput: Input {
public:
  SFMLMediaLayer * mediaLayer;
  SFMLInput(SFMLMediaLayer * mediaLayer);
  virtual ~SFMLInput();
  void eventLoop();

};
