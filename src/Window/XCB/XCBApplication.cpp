/*
 * Copyright © 2010 Lubosz Sarnecki
 * XCBApplication.cpp
 *
 *  Created on: Aug 21, 2011
 */

#include "XCBApplication.h"
#include "System/Timer.h"

XCBApplication::XCBApplication(int argc, char ** argv) {
  LogInfo << argc << argv[0];
  argcount = argc;
  argvalues = argv;
  app = new QApplication(argcount,argvalues, false);
  fontOverlay = false;
}

XCBApplication::~XCBApplication() {
    if (fontOverlay)
        delete gui;
    delete window;
    delete app;
}

void XCBApplication::updateFont() {
  gui->update();
}

void XCBApplication::draw() {
    renderFrame();
    if (fontOverlay)
      gui->draw();
    window->swapBuffers();
    //TODO: Mouse Input is buggy when title is updated less often
    window->updateWindowTitle();
    Timer::Instance().updateFPS();
}

void XCBApplication::run() {
  Config::Instance().load("config.xml");
  // Qt requires at least one argument.
  if (app->arguments().length() < 1) {
    LogError << "no args in qt app.";
  }

  window = new XCBWindow();
  window->init(Scene::Instance().name);

  scene();

  connect(window->getInput(), SIGNAL(shutdown()), app, SLOT(quit()));

  if (fontOverlay) {
    gui = new FontOverlay();
    gui->init();
    QTimer *fontTimer = new QTimer(this);
    connect(fontTimer, SIGNAL(timeout()), this, SLOT(updateFont()));
    fontTimer->start(1000);
  }

  QTimer *drawTimer= new QTimer(this);
  connect(drawTimer, SIGNAL(timeout()), this, SLOT(draw()));
  drawTimer->start(0);

  app->exec();
}

void XCBApplication::setFontOverlay(bool fontOverlay) {
  this->fontOverlay = fontOverlay;
}

