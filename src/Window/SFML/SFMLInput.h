/*
 * Copyright © 2010 Lubosz Sarnecki
 * Input.h
 *
 *  Created on: Sep 28, 2010
 */

#pragma once

#include "Window/Input.h"

class SFMLWindow;

class SFMLInput: Input {
public:
  SFMLWindow * mediaLayer;
  SFMLInput(SFMLWindow * mediaLayer);
  virtual ~SFMLInput();
  void eventLoop();

};
