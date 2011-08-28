/*
 * Copyright © 2010 Lubosz Sarnecki
 * Input.h
 *
 *  Created on: Sep 28, 2010
 */

#pragma once

#include "Window/Input.h"

class QtInput: public Input {
public:
  QtInput();
  virtual ~QtInput();
  void eventLoop();
};
