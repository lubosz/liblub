/*
 * Copyright © 2010 Lubosz Sarnecki
 * Application.cpp
 *
 *  Created on: Aug 21, 2011
 */

#include "Application.h"
#include "Timer.h"

void Application::chooseMediaLayer(MediaLayerType type) {
  if (type == windowXCB) {
#ifdef WITH_XCB
    mediaLayer = new XCBMediaLayer();
    LogInfo << "Using XCB for Input and Window";
#endif
  } else if (type == windowQt) {
#ifdef WITH_Qt
    mediaLayer = new QtMediaLayer();
    LogInfo << "Using Qt for Input and Window";
#endif
  } else if (type == windowSFML) {
#ifdef WITH_SFML
    mediaLayer = new SFMLMediaLayer();
    LogInfo << "Using SFML for Input and Window";
#endif
  } else if (type == windowSDL) {
#ifdef WITH_SDL
    mediaLayer = new SDLMediaLayer();
    LogInfo << "Using SDL for Input and Window";
#endif
  }
}

void Application::run() {
#if WITH_XCB
  run(windowXCB);
#elif WITH_SDL
  run(windowSDL);
#elif WITH_SFML
  run(windowSFML);
  //#elif WITH_Qt
  //    run(windowQt);
#else
  LogError << "No Media Layer compiled.";
#endif
}

void Application::updateFont() {
  gui->update();
}

void Application::draw() {
    renderFrame();
    if (fontOverlay)
      gui->draw();
    mediaLayer->swapBuffers();
    //TODO: Mouse Input is buggy when title is updated less often
    mediaLayer->updateWindowTitle();
    Timer::Instance().updateFPS();
}

void Application::eventLoop() {
  mediaLayer->input->eventLoop();
}

void Application::run(MediaLayerType type) {
  Config::Instance().load("config.xml");
  chooseMediaLayer(type);
  mediaLayer->init(SceneData::Instance().name);
  scene();

  connect(mediaLayer->input, SIGNAL(shutdown()), this, SLOT(quit()));

  if (fontOverlay) {
    gui = new GUI();
    gui->init();
    QTimer *fontTimer = new QTimer(this);
    connect(fontTimer, SIGNAL(timeout()), this, SLOT(updateFont()));
    fontTimer->start(1000);
  }

  QTimer *drawTimer= new QTimer(this);
  connect(drawTimer, SIGNAL(timeout()), this, SLOT(draw()));
  drawTimer->start(0);

  QTimer *eventTimer = new QTimer(this);
  connect(eventTimer, SIGNAL(timeout()), this, SLOT(eventLoop()));
  eventTimer->start(0);
  exec();
}

void Application::setFontOverlay(bool fontOverlay) {
  this->fontOverlay = fontOverlay;
}

