/*
 * PlaneMoon.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "PlaneMoon.h"
#include "Material/Textures.h"
#include "Material/ProcTextures.h"
#include "Material/Materials.h"
#include "System/TemplateEngine.h"
#include "Atmosphere.h"
#include "Scene/SceneData.h"

PlaneMoon::PlaneMoon(float innerRadius, float outerRadius) {
  this->innerRadius = innerRadius;
  this->outerRadius = outerRadius;
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

void PlaneMoon::init(const QVector3D& position, float size) {
  QList<string> attributes;
   attributes.push_back("normal");
   attributes.push_back("uv");
   this->position = position;

  Texture * glow = new TextureQImage(
      ProcTextures::makeGlow(QSize(512, 512), 40.0f, 0.1f), "glow");
  spaceFromAtmosphere = new Template("Atmo/Space", attributes);
  spaceFromAtmosphere->addTexture(glow);
  TemplateEngine::Instance().c.insert("fromSpace", true);
  spaceFromSpace = new Template("Atmo/Space", attributes);
  spaceFromSpace->addTexture(glow);

  spaceNode = new Node("space", position, size, moonPlane(attributes), spaceFromAtmosphere);
//  Atmosphere::setAtmoUniforms(spaceFromAtmosphere->getShaderProgram(), innerRadius, outerRadius);
//  Atmosphere::setAtmoUniforms(spaceFromSpace->getShaderProgram(), innerRadius, outerRadius);
}

void PlaneMoon::draw() {
  bool drawSpace = false;
  if (SceneData::Instance().getCurrentCamera()->position.length() < outerRadius) {
    spaceNode->setMaterial(spaceFromAtmosphere);
    drawSpace = true;
  } else if (SceneData::Instance().getCurrentCamera()->position.z() > 0.0f) {
    spaceNode->setMaterial(spaceFromSpace);
    drawSpace = true;
  }

  if (drawSpace) {
    SceneData::Instance().getCurrentCamera()->setUniforms(spaceNode->getMaterial()->getShaderProgram(), position);
    spaceNode->setView(SceneData::Instance().getCurrentCamera());
    spaceNode->draw();
  }
}
