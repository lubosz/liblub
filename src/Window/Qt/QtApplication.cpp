/*
 * QtApplication.cpp
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#include "QtApplication.h"

#include "Scene/Scene.h"
#include "System/Config.h"
#include "Renderer/OpenGL.h"
#include "QtInput.h"

QtApplication::QtApplication(int &argc, char **argv) : QApplication(argc, argv){
  Config::Instance().load("config.xml");
}

QtApplication::~QtApplication() {
}

void QtApplication::run() {
  // Qt requires at least one argument.
  if (arguments().length() < 1) {
    LogWarning << "No args for QtApplication.";
  }

  window = new QtWindow();

  //TODO hardcoded sizes
  window->setMaximumSize(QSize(1920, 1200));
  window->resize(QSize(1920, 1200));
  Scene::Instance().setResolution(1920,1200);

// TODO: Qt Makes errors when context version is specified
//  QGLFormat fmt = QGLFormat::defaultFormat();
//  vector<int> glContext = Config::Instance().values<int>("GLcontext");
//  fmt.setVersion(glContext[0], glContext[1]);
//  fmt.setVersion(3, 3);
//  fmt.setSwapInterval(0);
//  QGLFormat::OpenGL_Version_3_3;
//  LogInfo << "Supported GL Versions" << QGLFormat::openGLVersionFlags ();

  QGLFormat fmt;
  fmt.setDoubleBuffer(true);
  fmt.setProfile(QGLFormat::CoreProfile);
  glWidget = new GLWidget(fmt, window->splitter);
  glWidget->setFocus();
  if(!glWidget->isValid())
    LogFatal << "GL Widget Invalid";
  window->splitter->addWidget(glWidget);
  window->show();
#ifdef USE_GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    LogError << "GLEW Init:" <<glewGetErrorString(err);
  }
  LogInfo << "Using GLEW" << glewGetString(GLEW_VERSION);
#ifdef USE_WINDOWS
//  initWinGL();
#endif
#endif
  OpenGL::Instance().setContextCreated(true);
  fontOverlay = new FontOverlay();
  fontOverlay->init();
  scene();
  drawTimer = new QTimer(this);
  QtInput *input = &QtInput::Instance();
  connect(drawTimer, SIGNAL(timeout()), glWidget, SLOT(updateGL()));
  connect(input, SIGNAL(draw(void)), glWidget, SLOT(updateGL()));
  connect(glWidget, SIGNAL(draw()), this, SLOT(draw()));
//  app->exec();
  setLazy(false);
  exec();
}

void QtApplication::draw() {
  renderFrame();
  fontOverlay->draw();
  glError;
}

void QtApplication::setWireframe(bool wire) {
  useWireframe = wire;
  OpenGL::Instance().setWire(wire);
  glWidget->updateGL();
}

void QtApplication::setLazy(bool lazy){
  if(lazy){
    drawTimer->stop();
  }else{
    drawTimer->start(0);
  }
}
