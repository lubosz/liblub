/*
 * Atmosphere.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include <math.h>
#include "Atmosphere.h"
#include "System/TemplateEngine.h"
#include "Procedural/Geometry.h"
#include "Scene/Scene.h"
#include "Planet.h"

Atmosphere::Atmosphere(Planet * planet) {
  this->planet = planet;
}

Atmosphere::~Atmosphere() {
  // TODO Auto-generated destructor stub
}

void Atmosphere::init() {
  QList<string> attributes = QList<string>() << "normal" << "uv";
  initMaterials("Atmo/Sky", attributes);
  Mesh * outerSphere = Geometry::sphere(attributes, planet->outerRadius, 300, 500);
  node = new Node("sky", planet->position, planet->getSize(), outerSphere, fromAtmosphere);
}

void Atmosphere::draw() {
  checkMaterialToggle();

  Scene::Instance().getCurrentCamera()->setUniforms(
      node->getShader(), planet->position);
  node->setView(Scene::Instance().getCurrentCamera());
  node->draw();
}
