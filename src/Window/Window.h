/*
 * Copyright © 2010 Lubosz Sarnecki
 * MediaLayer.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include <string>
#include "Input.h"
#include "System/Logger.h"

using std::string;

namespace LL {
  class Window {

    friend class Input;
    friend class XCBInput;
    friend class SDLInput;

  public:

    bool quit;
    bool fullscreen;
    bool grab;
    string programTile;
    unsigned width, height;

    virtual void toggleFullScreen() = 0;
    virtual void toggleMouseGrab() = 0;
    virtual void mouseLook(int x, int y) = 0;
    virtual void init(string title) = 0;
    virtual void swapBuffers() = 0;
    virtual void updateWindowTitle() = 0;

    Input * getInput() {
      if(!input)
        LogFatal << "Input not initialized.";
      return input;
    }

  protected:
    Input * input;
    void shutdown() {
      quit = true;
    }
  };
}
