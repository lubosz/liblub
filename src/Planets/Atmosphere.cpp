/*
 * Atmosphere.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include <math.h>
#include "Atmosphere.h"
#include "Material/Materials.h"
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
  QList<string> attributes;
   attributes.push_back("normal");
   attributes.push_back("uv");

   TemplateEngine::Instance().c.insert("fromSpace", false);
  skyFromAtmosphere = new Template("Atmo/Sky",attributes);
  TemplateEngine::Instance().c.insert("fromSpace", true);
  skyFromSpace = new Template("Atmo/Sky",attributes);
  Mesh * outerSphere = Geometry::sphere(attributes, planet->outerRadius, 300, 500);
  node = new Node("sky", planet->position, planet->getSize(), outerSphere, skyFromAtmosphere);
  setAtmoUniforms(skyFromAtmosphere->getShaderProgram());
  setAtmoUniforms(skyFromSpace->getShaderProgram());
}

void Atmosphere::draw() {
  QVector3D camFromPlanet = SceneData::Instance().getCurrentCamera()->position
      - planet->position;
  float camDistance = camFromPlanet.length();
  if (camDistance >= planet->outerRadius) {
    node->setMaterial(skyFromSpace);
    updateWaveLength();
  } else {
    node->setMaterial(skyFromAtmosphere);
    updateWaveLength();
  }

  SceneData::Instance().getCurrentCamera()->setUniforms(
      node->getMaterial()->getShaderProgram(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());
  node->draw();
}
