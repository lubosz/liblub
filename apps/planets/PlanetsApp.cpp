/*
 * PlanetsApp.cpp
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#include "PlanetsApp.h"

#include "Scene/SceneData.h"
#include "Material/Textures.h"
#include "Material/Materials.h"
#include "System/Config.h"
#include "System/GUI.h"
#include "Renderer/RenderEngine.h"
#include "Window/Qt/FloatEditorWidget.h"
#include "Renderer/FrameBuffer.h"

PlanetsApp::PlanetsApp(int &argc, char **argv) :
  QtApplication(argc, argv) {

  usePostprocessing = true;
  useWireframe = false;

  focusedPlanet = new Planet("Earth", 11, 11.55, Planet::sun,
      { 0.650f, 0.570f, 0.475f }, { 0, 0, 0 }, 1);
  planets.push_back(focusedPlanet);
}

PlanetsApp::~PlanetsApp() {
}

PlanetWidget * PlanetsApp::focusPlanet() {
  PlanetWidget * planetWidget = new PlanetWidget(focusedPlanet);
  connect(planetWidget, SIGNAL(draw(void)), glWidget, SLOT(updateGL()));
  return planetWidget;
}

void PlanetsApp::setWireframe(bool wire) {
  useWireframe = wire;
  RenderEngine::Instance().setWire(wire);
  glWidget->updateGL();
}
void PlanetsApp::setPostprocessing(bool post) {
  usePostprocessing = post;
  glWidget->updateGL();
}

void PlanetsApp::setExposure(double exposure) {
  HDR->shaderProgram->use();
  HDR->shaderProgram->setUniform("exposure", (float) exposure);
  glWidget->updateGL();
}

void PlanetsApp::scene() {
    initWidgets(window->mainLayout);
  window->setWindowTitle("Planets Demo");
  initCamAndLight();
  foreach(Planet * planet, planets)
      planet->init();
  initPostProcessing();
}

void PlanetsApp::renderFrame() {
  startPass();
  RenderEngine::Instance().clear();
  drawPlanets();
  endPass();
}

void PlanetsApp::initCamAndLight() {
  camera = SceneData::Instance().getCurrentCamera();
  camera->setPosition(QVector3D(0, 11.1, -0.85));
  light = new Light(QVector3D(0, 0, 1000), QVector3D(1, -5, 0));
  SceneData::Instance().addLight("sunlight", light);

  camera->setRotationX(2.9);
  camera->setRotationY(176.6);
}

void PlanetsApp::initWidgets(QHBoxLayout * mainLayout) {
  QVBoxLayout *sideLayout = new QVBoxLayout;
  mainLayout->addLayout(sideLayout);

  QListWidget * planetList = new QListWidget();

  foreach(Planet * planet , planets) {
      QListWidgetItem * planetItem = new QListWidgetItem(planetList);
      planetItem->setText(planet->name);
  }
  QCheckBox *postBox = new QCheckBox();
  postBox->setText("Postprocessing");
  postBox->setChecked(true);
  connect(postBox, SIGNAL(clicked(bool)), this, SLOT(setPostprocessing(bool)));
  sideLayout->addWidget(postBox);

  FloatEditorWidget* exposureWidget = new FloatEditorWidget("Exposure", SLOT(setExposure(double)), 2.0, 0, 10, this);
  connect(exposureWidget, SIGNAL(draw()), glWidget, SLOT(updateGL()));
  sideLayout->addWidget(exposureWidget);

  QCheckBox *wireFrameBox = new QCheckBox();
  wireFrameBox->setText("Wireframe");
  wireFrameBox->setChecked(false);
  connect(wireFrameBox, SIGNAL(clicked(bool)), this, SLOT(setWireframe(bool)));
  sideLayout->addWidget(wireFrameBox);

  QCheckBox *lazyBox = new QCheckBox();
  lazyBox->setText("Lazy Rendering");
  lazyBox->setChecked(true);
  connect(lazyBox, SIGNAL(clicked(bool)), this, SLOT(setLazy(bool)));
  sideLayout->addWidget(lazyBox);

  sideLayout->addWidget(focusPlanet());
}

void PlanetsApp::initPostProcessing() {
  //TODO: Hardcoded res
  QSize res(1920, 1200);
  fbo = new FrameBuffer(res);
  Texture * targetTexture = new ColorTexture(res, "targetTexture");
  fbo->attachTexture(targetTexture);
  fbo->check();

  HDR = new Template("Post/HDR", QList<string> () << "uv");
  HDR->addTexture(targetTexture);
  HDR->shaderProgram->setUniform("exposure", 2.0f);
}

void PlanetsApp::startPass() {
  if (usePostprocessing && !useWireframe) {
    fbo->bind();
    fbo->updateRenderView();
  }
}

void PlanetsApp::endPass() {
  if (usePostprocessing && !useWireframe) {
    fbo->unBind();
    RenderEngine::Instance().updateViewport(glWidget->viewSize);
    RenderEngine::Instance().clear();
    HDR->activateTextures();
    HDR->getShaderProgram()->use();
    fbo->draw(HDR);
  }
}

void PlanetsApp::drawPlanets() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  foreach(Planet * planet, planets)
      planet->draw();

  glFrontFace(GL_CW);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  //sort atmospheres
  QMap<qreal, Atmosphere*> atmoSpheres;

  foreach(Planet * planet, planets) {
      QVector3D distance = planet->atmoSphere->node->getPosition()
          - SceneData::Instance().getCurrentCamera()->position;
      atmoSpheres.insert(distance.length(), planet->atmoSphere);
  }

  QList<qreal> depthSortKeys = atmoSpheres.keys();
  qSort(depthSortKeys.begin(), depthSortKeys.end(), qGreater<qreal> ());

  foreach(qreal depthSortKey, depthSortKeys) {
      atmoSpheres[depthSortKey]->draw();
  }

  glDisable(GL_BLEND);
  glFrontFace(GL_CCW);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
}
