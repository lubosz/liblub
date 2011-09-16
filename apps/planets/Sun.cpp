/*
 * Sun.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "Sun.h"
#include "System/Timer.h"
#include "Scene/SceneData.h"
#include "Mesh/Geometry.h"
#include "Planet.h"
#include "Material/Textures.h"

Sun::Sun(Planet * planet) {
  this->planet = planet;
}

Sun::~Sun() {
  // TODO Auto-generated destructor stub
}

void Sun::init(){
  QList<string> attributes = QList<string>() << "normal" << "uv";

  initMaterials("Atmo/Sun", attributes);

  Texture * mars = new TextureFile("Planets/Mars.jpg","planet");
  fromAtmosphere->addTexture(mars);
  fromSpace->addTexture(mars);
  node = new Node("sun", planet->position, planet->getSize(), Geometry::sphere(attributes, planet->innerRadius, 800, 400), fromAtmosphere);
  node->setRotation(QVector3D(-90,0,180));
}
void Sun::draw(){
  checkMaterialToggle();
  node->getShader()->use();
  node->getShader()->setUniform("time", Timer::Instance().getTime());
  SceneData::Instance().getCurrentCamera()->setUniforms(
      node->getShader(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());
  node->draw();
}
