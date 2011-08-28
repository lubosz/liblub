/*
 * Copyright © 2010 Lubosz Sarnecki
 * Application.cpp
 *
 *  Created on: Aug 21, 2011
 */

#include "Application.h"
#include "Timer.h"

Application::Application(int argc, char ** argv) {
  LogInfo << argc << argv[0];
  argcount = argc;
  argvalues = argv;
  app = new QApplication(argcount,argvalues, false);
}

void Application::chooseMediaLayer(WindowType type) {
  if (type == windowXCB) {
#ifdef WITH_XCB
    window = new XCBWindow();
    LogInfo << "Using XCB for Input and Window";
#endif
  } else if (type == windowQt) {
#ifdef WITH_Qt
    window = new QtWindow();
    LogInfo << "Using Qt for Input and Window";
#endif
  } else if (type == windowSFML) {
#ifdef WITH_SFML
    window = new SFMLWindow();
    LogInfo << "Using SFML for Input and Window";
#endif
  } else if (type == windowSDL) {
#ifdef WITH_SDL
    window = new SDLWindow();
    LogInfo << "Using SDL for Input and Window";
#endif
  }
}

void Application::run() {
  Config::Instance().load("config.xml");

#if WITH_XCB
  run(windowXCB);
#elif WITH_SDL
  run(windowSDL);
#elif WITH_SFML
  run(windowSFML);
#elif WITH_Qt
  run(windowQt);
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
    window->swapBuffers();
    //TODO: Mouse Input is buggy when title is updated less often
    window->updateWindowTitle();
    Timer::Instance().updateFPS();
}

void Application::eventLoop() {
  window->getInput()->eventLoop();
}

void Application::run(WindowType type) {
  // Qt requires at least one argument.
  if (app->arguments().length() < 1) {
    LogWarning << "Oh noez, no argz. Better append foo.";
    app->arguments().push_back("foo");
  }
  LogInfo << app->arguments().at(0).toStdString();

  chooseMediaLayer(type);
  window->init(SceneData::Instance().name);

  scene();

  connect(window->getInput(), SIGNAL(shutdown()), app, SLOT(quit()));

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
  app->exec();
}

void Application::setFontOverlay(bool fontOverlay) {
  this->fontOverlay = fontOverlay;
}

