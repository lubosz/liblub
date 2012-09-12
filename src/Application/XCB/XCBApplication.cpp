/*
 * Copyright © 2010 Lubosz Sarnecki
 * XCBApplication.cpp
 *
 *  Created on: Aug 21, 2011
 */

#include "XCBApplication.h"
#include "System/Timer.h"

XCBApplication::XCBApplication(int argc, char ** argv) {
  argcount = argc;
  argvalues = argv;
  app = new QApplication(argcount,argvalues, false);
  useFontOverlay = false;
}

XCBApplication::~XCBApplication() {
    if (useFontOverlay)
        delete fontOverlay;
    delete window;
    delete app;
}

void XCBApplication::updateFont() {
  fontOverlay->update();
}

void XCBApplication::draw() {
    renderFrame();
    if (useFontOverlay)
      fontOverlay->draw();
    window->update();
}

void XCBApplication::run() {
  // Qt requires at least one argument.
  if (app->arguments().length() < 1) {
    LogError << "no args in qt app.";
  }

  window = new XCBWindow();
  window->init(Scene::Instance().name);

  scene();

  connect(window->getInput(), SIGNAL(shutdown()), app, SLOT(quit()));

  if (useFontOverlay) {
    fontOverlay = new FontOverlay();
    fontOverlay->init();
    QTimer *fontTimer = new QTimer(this);
    connect(fontTimer, SIGNAL(timeout()), this, SLOT(updateFont()));
    fontTimer->start(1000);
  }

  QTimer *drawTimer= new QTimer(this);
  connect(drawTimer, SIGNAL(timeout()), this, SLOT(draw()));
  drawTimer->start(0);

  app->exec();
}

void XCBApplication::setFontOverlay(bool useFontOverlay) {
  this->useFontOverlay = useFontOverlay;
}

