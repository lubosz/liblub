/*
 * PlaneMoon.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "PlaneMoon.h"
#include "Texture/Textures.h"
#include "Procedural/ProcTextures.h"
#include "System/TemplateEngine.h"
#include "Atmosphere.h"
#include "Scene/Scene.h"
#include "Planet.h"
#include "Procedural/Geometry.h"

PlaneMoon::PlaneMoon(Planet * planet) {
  this->planet = planet;
}

PlaneMoon::~PlaneMoon() {
  // TODO Auto-generated destructor stub
}

Mesh * PlaneMoon::moonPlane(const QList<string> & attributes) {
  Mesh * moonPlane = new Mesh(attributes);
  moonPlane->buffers["position"] = {
    -4.0f, 4.0f, -50.0f,
    -4.0f, -4.0f, -50.0f,
    4.0f, -4.0f, -50.0f,
    4.0f, 4.0f, -50.0f
  };
  moonPlane->buffers["uv"] = {
    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    1.0, 0.0,
  };
  moonPlane->indices = {0, 1, 3, 3, 1, 2};
  moonPlane->init();
  moonPlane->setDrawType(GL_TRIANGLES);
  return moonPlane;
}

void PlaneMoon::init() {
//    Material * skyDomeMat = new Template("Texture", QList<string>() << "uv");

    QList<string> attributes = QList<string>() << "normal" << "uv";
    initMaterials("Atmo/Space", attributes);

    Texture * skyDomeMap = new TextureFile("Earth/StarsMap_2500x1250.jpg", "glow");
    fromAtmosphere->addTexture(skyDomeMap);
    fromSpace->addTexture(skyDomeMap);

//    Mesh * skyDomeMesh = Geometry::sphere(attributes, 1000, 100, 50);
    Mesh * skyDomeMesh = Geometry::sphere(attributes, 50, 100, 50);
//    Mesh * skyDomeMesh = moonPlane(attributes);
    node = new Node("space", QVector3D(0,0,0), 1, skyDomeMesh, fromAtmosphere);
//    node->setRotation(QVector3D(-90, 0, 0));


//  QList<string> attributes = QList<string>() << "normal" << "uv";
//  initMaterials("Atmo/Space", attributes);
//
//  Texture * glow = new TextureQImage(ProcTextures::makeGlow(QSize(512, 512), 40.0f, 0.1f), "glow");
//  fromAtmosphere->addTexture(glow);
//  fromSpace->addTexture(glow);
//
//  node = new Node("space", planet->position, planet->getSize(), moonPlane(attributes), fromAtmosphere);
//  setAtmoUniforms(fromAtmosphere->getShaderProgram());
//  setAtmoUniforms(fromSpace->getShaderProgram());
}

void PlaneMoon::draw() {
  checkMaterialToggle();
  Scene::Instance().getCurrentCamera()->setUniforms(
      node->getShader(), planet->position);
  node->setView(Scene::Instance().getCurrentCamera());
  glDisable(GL_CULL_FACE);
  node->draw();
  glEnable(GL_CULL_FACE);
}
