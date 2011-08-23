#include <QtGui>
#include <math.h>

#include "glWidget.h"
#include "System/GUI.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "Material/Materials.h"
#include "Material/Textures.h"
#include "Renderer/RenderEngine.h"
#include "System/Timer.h"
#include "Mesh/Geometry.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

  QGLFormat fmt;
  fmt.setVersion(4,1);
  QGLFormat::setDefaultFormat(fmt);

  usePostprocessing = true;
  useWireframe = false;
  //   wavelength[0] = 0.650f; // 650 nm for red
  //    wavelength[1] = 0.570f; // 570 nm for green
  //    wavelength[2] = 0.475f; // 475 nm for blue
//  planets.push_back(new Planet("Ocean", 11,11.55, Planet::ocean, {0.650f, 0.570f,0.475f},{50,0,0},1));
//  planets.push_back(new Planet("Sun",11,11.55, Planet::sun, {0.650f,1,0},{0,0,500},1));
//    planets.push_back(new Planet(11,11.55, Planet::terrainTess, {0.150f, 0.870f,0.175f},{-10,0,0},1));
//    planets.push_back(new Planet(11,11.55, Planet::terrainPlain, {0.650f, 0.570f,0.475f},{0,0,0},1));
  focusedPlanet = new Planet("Earth", 11,11.55, Planet::sun, {0.650f, 0.570f,0.475f},{0,0,0},1);
  planets.push_back(focusedPlanet);
//  timerId = startTimer(0);

//  m_fInnerRadius = 10.0f;
//  m_fOuterRadius = 10.25f;

}

GLWidget::~GLWidget() {
}
void GLWidget::setWireframe(bool wire){
  useWireframe = wire;
  RenderEngine::Instance().setWire(wire);
}
void GLWidget::setPostprocessing(bool post){
  usePostprocessing = post;
  updateGL();
}

void GLWidget::setExposure(double exposure){
  HDR->shaderProgram->use();
  HDR->shaderProgram->setUniform("exposure", (float) exposure);
  updateGL();
}
QSize GLWidget::minimumSizeHint() const {
  return QSize(800, 600);
}

QSize GLWidget::sizeHint() const {
  return QSize(1720, 1200);
}

void GLWidget::initializeGL() {
#ifdef USE_GLEW
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

//  SceneLoader * sceneLoader = new SceneLoader("planets.xml");
//  sceneLoader->load();
  initCamAndLight();
  gui = new GUI();
  gui->init();
  foreach(Planet * planet, planets)
      planet->init();
  initPostProcessing();
}

void GLWidget::paintGL() {
//  RenderEngine::Instance().clear();
  startPass();
  RenderEngine::Instance().clear();
  drawPlanets();
//  SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
  endPass();
  gui->draw();
  glError;

}

void GLWidget::resizeGL(int width, int height) {
  SceneData::Instance().getCurrentCamera()->setAspect(
      float(width) / float(height));
  glViewport(0, 0, width, height);
  viewSize = QSize(width, height);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  this->setFocus();
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    SceneData::Instance().getCurrentCamera()->setMouseLook(dx, dy, .1);
  } else if (event->buttons() & Qt::RightButton) {
  }
  lastPos = event->pos();
  updateGL();
}

 void GLWidget::initCamAndLight(){
   camera = SceneData::Instance().getCurrentCamera();
//    camera->setPosition(QVector3D(0, 10, 7));
   camera->setPosition(QVector3D(0, 11.1, -0.85));
   camera->update();
   light = new Light(QVector3D(0,0,1000), QVector3D(1, -5, 0));
   SceneData::Instance().addLight("sunlight", light);

   camera->yaw = 2.9;
   camera->pitch = 176.6;
   camera->update();
   camera->updateView();
   camera->updateRotation();
   camera->updatePerspective();
 }

 void GLWidget::initPostProcessing() {
  //TODO: Hardcoded res
  QSize res(1920, 1200);
  fbo = new FrameBuffer(res);
  Texture * targetTexture = new ColorTexture(res, "targetTexture");
  fbo->attachTexture(GL_COLOR_ATTACHMENT0, targetTexture);
  fbo->check();

  HDR = new Template("Post/HDR", QList<string> () << "uv");
  HDR->addTexture(targetTexture);
  HDR->shaderProgram->setUniform("exposure", 2.0f);
}

 void GLWidget::startPass(){
   if(usePostprocessing && !useWireframe) {
     fbo->bind();
     fbo->updateRenderView();
   }
 }

 void GLWidget::endPass(){
   if(usePostprocessing && !useWireframe){
     fbo->unBind();
     glViewport(0, 0, viewSize.width(),viewSize.height());
     RenderEngine::Instance().clear();
     HDR->activateTextures();
     HDR->getShaderProgram()->use();
     fbo->draw(HDR);
   }
 }

 void GLWidget::drawPlanets() {
  glEnable( GL_DEPTH_TEST);
  glEnable( GL_CULL_FACE);

  foreach(Planet * planet, planets)
      planet->draw();

  glFrontFace( GL_CW);
  glEnable( GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  //sort atmospheres
  QMap <qreal, Atmosphere*> atmoSpheres;

  foreach(Planet * planet, planets) {
    QVector3D distance = planet->atmoSphere->node->position - SceneData::Instance().getCurrentCamera()->position;
    atmoSpheres.insert(distance.length(), planet->atmoSphere);
  }

  QList<qreal> depthSortKeys = atmoSpheres.keys();
  qSort(depthSortKeys.begin(), depthSortKeys.end(), qGreater<qreal> ());

  foreach(qreal depthSortKey, depthSortKeys) {
    atmoSpheres[depthSortKey]->draw();
  }

  glDisable(GL_BLEND);
  glFrontFace( GL_CCW);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
}

 void GLWidget::setLazy(bool lazy){
   if(lazy){
       killTimer(timerId);
   }else{
       timerId = startTimer(0);
   }

 }

