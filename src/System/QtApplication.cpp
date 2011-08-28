/*
 * QtApplication.cpp
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#include "QtApplication.h"

#include "Scene/SceneData.h"
#include "System/Config.h"
#include "System/GUI.h"
#include "Renderer/RenderEngine.h"

QtApplication::QtApplication(int &argc, char **argv) :
  QApplication(argc, argv) {
  Config::Instance().load("config.xml");
}

QtApplication::~QtApplication() {
}

void QtApplication::run() {
  window = new QtWindow();
  window->setMaximumSize(QSize(1920, 1200));
  window->resize(QSize(1920, 1200));

  glWidget = new GLWidget;
  glWidget->setFocus();

  initWidgets(window->mainLayout);
  window->show();
  RenderEngine::Instance();
  gui = new GUI();
  gui->init();
  scene();
  drawTimer = new QTimer(this);
  connect(drawTimer, SIGNAL(timeout()), glWidget, SLOT(updateGL()));
  connect(window, SIGNAL(draw(void)), glWidget, SLOT(updateGL()));
  connect(glWidget, SIGNAL(draw()), this, SLOT(draw()));
  exec();
}

void QtApplication::draw() {
  renderFrame();
  gui->draw();
  glError;
}

void QtApplication::setLazy(bool lazy){
  if(lazy){
    drawTimer->stop();
  }else{
    drawTimer->start(0);
  }
}
