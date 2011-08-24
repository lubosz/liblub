/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include <iostream>
#include <sstream>
#include "SFMLMediaLayer.h"
#include "SFMLInput.h"
#include "System/Config.h"

SFMLWindow::SFMLWindow() {

  //FPS Stuff
  /*
   fps_lasttime = 0; //the last recorded time.
   fps_frames = 0; //frames passed since the last recorded fps.
   */
  fullscreen = false;
  grab = false;
  quit = false;
  input = (Input*) new SFMLInput(this);
}

void SFMLWindow::init(string title) {
  programTile = title;
  //sf::RenderWindow window(sf::VideoMode(800, 600), title);
  sf::ContextSettings settings;
  vector<int> glContext = Config::Instance().values<int> ("GLcontext");
  settings.MajorVersion = glContext[0];
  settings.MinorVersion = glContext[1];
  /*
   settings.DepthBits         = 24; // Request a 24 bits depth buffer
   settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
   settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing
   */

  window = new sf::RenderWindow(sf::VideoMode::GetFullscreenModes().at(0),
      title, sf::Style::Fullscreen, settings);
}

SFMLWindow::~SFMLWindow() {
}

void SFMLWindow::swapBuffers() {
  window->Display();
}

void SFMLWindow::toggleFullScreen() {
  if (fullscreen) {
    printf("Fullscreen Off\n");

    fullscreen = false;
  } else {
    printf("Fullscreen On\n");

    fullscreen = true;
  }

}

void SFMLWindow::toggleMouseGrab() {

}
void SFMLWindow::mouseLook(int x, int y) {

}
void SFMLWindow::renderFrame() {

}
