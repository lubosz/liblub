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

PlaneMoon::PlaneMoon() {
  // TODO Auto-generated constructor stub
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
  QList<string> attributes;
   attributes.push_back("normal");
   attributes.push_back("uv");

  Texture * glow = new TextureQImage(
      ProcTextures::makeGlow(QSize(512, 512), 40.0f, 0.1f), "glow");
  spaceFromAtmosphere = new Template("Atmo/Space", attributes);
  spaceFromAtmosphere->addTexture(glow);
  TemplateEngine::Instance().c.insert("fromSpace", true);
  spaceFromSpace = new Template("Atmo/Space", attributes);
  spaceFromSpace->addTexture(glow);

  spaceNode = new Node("space", { 0, 0, 0 }, 1, moonPlane(attributes), spaceFromAtmosphere);
  setAtmoUniforms(spaceFromAtmosphere->getShaderProgram());
  setAtmoUniforms(spaceFromSpace->getShaderProgram());
}

void PlaneMoon::draw(Camera * camera, float outerRadius) {
  bool drawSpace = false;
  if (camera->position.length() < outerRadius) {
    spaceNode->setMaterial(spaceFromAtmosphere);
    drawSpace = true;
  } else if (camera->position.z() > 0.0f) {
    spaceNode->setMaterial(spaceFromSpace);
    drawSpace = true;
  }

  if (drawSpace) {
    setCameraUniforms(spaceNode->getMaterial()->getShaderProgram());
    spaceNode->setView(camera);
    spaceNode->draw();
  }
}
