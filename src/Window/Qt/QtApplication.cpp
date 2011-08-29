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

QtApplication::QtApplication(int & argc, char ** argv) {
  Config::Instance().load("config.xml");
  // Cache args over App execution time
  myargc = argc;
  myargv = argv;
  app = new QApplication(myargc, myargv);
}

QtApplication::~QtApplication() {
}

void QtApplication::run() {
  // Qt requires at least one argument.
  if (app->arguments().length() < 1) {
    LogFatal << "No args for QtApplication.";
  }

  window = new QtWindow();
  //TODO hardcoded sizes
  window->setMaximumSize(QSize(1920, 1200));
  window->resize(QSize(1920, 1200));
  SceneData::Instance().setResolution(1920,1200);

  glWidget = new GLWidget;
  glWidget->setFocus();

  window->mainLayout->addWidget(glWidget);
  initWidgets(window->mainLayout);
  window->show();
#ifdef LIBLUB_WINDOWS
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    LogError << "GLEW Init:" <<glewGetErrorString(err);
  }
  LogInfo << "Using GLEW" << glewGetString(GLEW_VERSION);
  initWinGL();
#endif
  RenderEngine::Instance();
  gui = new GUI();
  gui->init();
  scene();
  drawTimer = new QTimer(this);
  connect(drawTimer, SIGNAL(timeout()), glWidget, SLOT(updateGL()));
  connect(window, SIGNAL(draw(void)), glWidget, SLOT(updateGL()));
  connect(glWidget, SIGNAL(draw()), this, SLOT(draw()));
  app->exec();
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
