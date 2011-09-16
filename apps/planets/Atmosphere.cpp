/*
 * Atmosphere.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include <math.h>
#include "Atmosphere.h"
#include "System/TemplateEngine.h"
#include "Mesh/Geometry.h"
#include "Scene/SceneData.h"
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

  SceneData::Instance().getCurrentCamera()->setUniforms(
      node->getShader(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());
  node->draw();
}
