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

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

  QGLFormat fmt;
  fmt.setVersion(4,1);
  QGLFormat::setDefaultFormat(fmt);
  useHDR = false;
  //   wavelength[0] = 0.650f; // 650 nm for red
  //    wavelength[1] = 0.570f; // 570 nm for green
  //    wavelength[2] = 0.475f; // 475 nm for blue
  planets.push_back(new Planet(11,11.55, Planet::ocean, {0.650f, 0.570f,0.475f},{50,0,0},1));
  planets.push_back(new Planet(11,11.55, Planet::sun, {0.650f,1,0},{0,0,500},1));
//    planets.push_back(new Planet(11,11.55, Planet::terrainTess, {0.150f, 0.870f,0.175f},{-10,0,0},1));
//    planets.push_back(new Planet(11,11.55, Planet::terrainPlain, {0.650f, 0.570f,0.475f},{0,0,0},1));
  earth = new Planet(11,11.55, Planet::terrainPlain, {0.150f, 0.570f,0.475f},{0,0,0},1);
  planets.push_back(earth);
}

GLWidget::~GLWidget() {
}

QSize GLWidget::minimumSizeHint() const {
  return QSize(800, 600);
}

QSize GLWidget::sizeHint() const {
  return QSize(1720, 1200);
}

void GLWidget::initializeGL() {
//  SceneLoader * sceneLoader = new SceneLoader("nice.xml");
//  sceneLoader->load();
  GUI::Instance().init();


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
  SceneData::Instance().getCurrentCamera()->setAspect(float(width) / float(height));
  glViewport(0, 0, width,height);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
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
  glEnable( GL_DEPTH_TEST);
  glEnable( GL_CULL_FACE);

  foreach(Planet * planet, planets)
      planet->draw();

  glFrontFace( GL_CW);
  glEnable( GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

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

