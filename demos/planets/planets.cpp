/*
 * PlanetsApp.cpp
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#include "planets.h"

#include "Scene/Scene.h"
#include "Texture/Textures.h"
#include "System/Config.h"
#include "Renderer/OpenGL.h"
#include "Window/Qt/FloatEditorWidget.h"
#include "Renderer/FrameBuffer.h"
#include "Shader/Shaders.h"
#include "Procedural/Geometry.h"
#include "Renderer/RenderPasses.h"

PlanetsDemo::PlanetsDemo() :
    Demo("planets") {

  usePostprocessing = true;
  useWireframe = false;

  focusedPlanet = new Planet("Earth", 11, 11.55, Planet::sun,
                             QVector3D(0.650f, 0.570f, 0.475f),
                             QVector3D( 0, 0, 0 ), 1);
  planets.push_back(focusedPlanet);

}

PlanetsDemo::~PlanetsDemo() {
}

void PlanetsDemo::init() {
  initCamAndLight();
  fullPlane = Geometry::plane(QList<string> () << "uv", QRectF(-1, -1, 2, 2));
  foreach(Planet * planet, planets)
      planet->init();
  initPostProcessing();
}

void PlanetsDemo::draw() {
  startPass();
  OpenGL::Instance().clear();
  drawPlanets();
  endPass();
}

void PlanetsDemo::initCamAndLight() {
  camera = Scene::Instance().getCurrentCamera();
  camera->setPosition(QVector3D(0, 11.1, -0.85));
  light = new Light(QVector3D(0, 0, 1000), QVector3D(1, -5, 0));
  Scene::Instance().addLight("sunlight", light);

  //TODO: Cam does not update
  camera->setRotationX(2.9);
  camera->setRotationY(176.6);
  camera->update();
}

void PlanetsDemo::initPostProcessing() {
  //TODO: Hardcoded res
  QSize res(1920, 1200);
  fbo = new FrameBuffer(res);
  Texture * targetTexture = new ColorTexture(res, "targetTexture");
  fbo->attachTexture(targetTexture);
  fbo->check();

  HDR = new VertFragProgram("Post/HDR", QList<string> () << "uv");
  HDR->addTexture(targetTexture);
  HDR->setUniform("exposure", 2.0f);
  HDR->samplerUniforms();
}

void PlanetsDemo::startPass() {
  if (usePostprocessing && !useWireframe) {
    fbo->bind();
    fbo->updateRenderView();
  }
}

void PlanetsDemo::endPass() {
  if (usePostprocessing && !useWireframe) {
    fbo->unBind();
//    OpenGL::Instance().updateViewport(glWidget->viewSize);
    QSize size = QSize(1920,1200);
    OpenGL::Instance().updateViewport(size);
    OpenGL::Instance().clear();
    HDR->activateTextures();
    HDR->bindTextures();
    HDR->use();
    DrawThing::drawOnPlane(HDR, fullPlane);
  }
}

void PlanetsDemo::drawPlanets() {
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
          - Scene::Instance().getCurrentCamera()->position;
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
