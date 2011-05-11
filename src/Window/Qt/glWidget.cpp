#include <QtGui>
#include <math.h>

#include "glWidget.h"
#include "System/GUI.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "Material/Materials.h"
#include "System/Config.h"
#include "Material/Textures.h"
#include "Renderer/RenderEngine.h"
#include "System/Timer.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
  xRot = 0;
  yRot = 0;
  zRot = 0;
  earthWaveLength = QVector3D();

  QGLFormat fmt;
  fmt.setVersion(4,1);
//  fmt.setAlpha(true);
//  fmt.setStereo(true);
  QGLFormat::setDefaultFormat(fmt);
}

GLWidget::~GLWidget() {
}

QSize GLWidget::minimumSizeHint() const {
  return QSize(800, 600);
}

QSize GLWidget::sizeHint() const {
  return QSize(1720, 1200);
}

static void qNormalizeAngle(int &angle) {
  while (angle < 0)
    angle += 360 * 16;
  while (angle > 360 * 16)
    angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != xRot) {
    xRot = angle;
    SceneData::Instance().getCurrentCamera()->yaw = xRot;
    SceneData::Instance().getCurrentCamera()->updateRotation();
    emit
    xRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::setYRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != yRot) {
    yRot = angle;
    SceneData::Instance().getCurrentCamera()->pitch = yRot;
    SceneData::Instance().getCurrentCamera()->updateRotation();
    emit
    yRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::setRed(int red){
  earthWaveLength.setX(float(red)/256.0);
  earth->updateWaveLength(earthWaveLength);
  updateGL();
}

void GLWidget::setGreen(int green){
  earthWaveLength.setY(float(green)/256.0);
  earth->updateWaveLength(earthWaveLength);
  updateGL();
}

void GLWidget::setBlue(int blue){
  earthWaveLength.setZ(float(blue)/256.0);
  earth->updateWaveLength(earthWaveLength);
  updateGL();
}

void GLWidget::setZRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != zRot) {
    zRot = angle;
    SceneData::Instance().getCurrentCamera()->roll = zRot;
    SceneData::Instance().getCurrentCamera()->updateRotation();
    emit
    zRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::initializeGL() {
  Config::Instance().load("config.xml");
  SceneLoader * sceneLoader = new SceneLoader("nice.xml");
  sceneLoader->load();
  GUI::Instance().init();

  //   wavelength[0] = 0.650f; // 650 nm for red
  //    wavelength[1] = 0.570f; // 570 nm for green
  //    wavelength[2] = 0.475f; // 475 nm for blue
  useHDR = false;
  planets.push_back(new Planet(11,11.55, Planet::ocean, {0.650f, 0.570f,0.475f},{50,0,0},1));
  planets.push_back(new Planet(11,11.55, Planet::sun, {0.650f,1,0},{0,0,50},1));
//    planets.push_back(new Planet(11,11.55, Planet::terrainTess, {0.150f, 0.870f,0.175f},{-10,0,0},1));
//    planets.push_back(new Planet(11,11.55, Planet::terrainPlain, {0.650f, 0.570f,0.475f},{0,0,0},1));

  earth = new Planet(11,11.55, Planet::terrainPlain, {0.150f, 0.570f,0.475f},{0,0,0},1);
  planets.push_back(earth);

  foreach(Planet * planet, planets)
      planet->init();
  initCamAndLight();
  initPostProcessing();
}

void GLWidget::paintGL() {
//  RenderEngine::Instance().clear();
//  SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
  Timer::Instance().frame();

  startPass();
  RenderEngine::Instance().clear();
  drawPlanets();
  endPass();
  GUI::Instance().draw();
  glError;

}

void GLWidget::resizeGL(int width, int height) {
//  int side = qMin(width, height);
  SceneData::Instance().getCurrentCamera()->setAspect(float(width) / float(height));
  glViewport(0, 0, width,height);
//  LogDebug << side;
//  glViewport((width - side) / 2, (height - side) / 2, side, side);
//  glMatrixMode( GL_PROJECTION);
//  glLoadIdentity();
//  glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
//  glMatrixMode( GL_MODELVIEW);
//  initPostProcessing();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  SceneData::Instance().getCurrentCamera()->setMouseLook(dx, dy, .1);
//  if (event->buttons() & Qt::LeftButton) {
//    setXRotation(xRot + 8 * dy);
//    setYRotation(yRot + 8 * dx);
//  } else if (event->buttons() & Qt::RightButton) {
//    setXRotation(xRot + 8 * dy);
//    setZRotation(zRot + 8 * dx);
//  }
  lastPos = event->pos();
  updateGL();
}

 void GLWidget::initCamAndLight(){
   camera = SceneData::Instance().getCurrentCamera();
//    camera->setPosition(QVector3D(0, 10, 7));
   camera->setPosition(QVector3D(0, 11.1, -0.85));
   camera->update();
   light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
   SceneData::Instance().addLight("foolight", light);

   camera->yaw = 2.9;
   camera->pitch = 176.6;
   camera->update();
   camera->updateView();
   camera->updateRotation();
   camera->updatePerspective();
 }

 void GLWidget::initPostProcessing(){
   if(useHDR){
     unsigned width = this->sizeHint().width();
     unsigned height = this->sizeHint().height();

     fbo = new FrameBuffer(width, height);
     Texture * targetTexture = new ColorTexture(width, height, "targetTexture");
     fbo->attachTexture(GL_COLOR_ATTACHMENT0, targetTexture);

     QList<string> attributes;
     attributes.push_back("uv");

     HDR = new Template("Post/HDR",attributes);
     HDR->addTexture(targetTexture);
     HDR->shaderProgram->setUniform("exposure", 2.0f);
     fbo->checkAndFinish();
   }
 }

 void GLWidget::startPass(){

//   useHDR = !RenderEngine::Instance().wire;

   if(useHDR) {
     fbo->bind();
     fbo->updateRenderView();
   }
 }

 void GLWidget::endPass(){
   if(useHDR){
     fbo->unBind();
     RenderEngine::Instance().clear();
     HDR->activateTextures();
     HDR->getShaderProgram()->use();
     fbo->draw(HDR);
   }
 }

 void GLWidget::drawPlanets() {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);

   foreach(Planet * planet, planets)
       planet->draw();

   glFrontFace(GL_CW);
   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE, GL_ONE);

   foreach(Planet * planet, planets)
       planet->atmoSphere->draw();

   glDisable(GL_BLEND);
   glFrontFace(GL_CCW);
   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);
 }

