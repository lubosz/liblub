/*
 * Sun.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "Sun.h"
#include "System/Timer.h"
#include "Scene/SceneData.h"
#include "Material/Materials.h"
#include "Mesh/Geometry.h"
#include "Planet.h"

Sun::Sun(Planet * planet) {
  this->planet = planet;
}

Sun::~Sun() {
  // TODO Auto-generated destructor stub
}

void Sun::init(){
  QList<string> attributes = QList<string>() << "normal" << "uv";

  initMaterials("Atmo/Sun", attributes);

  node = new Node("sun", planet->position, planet->getSize(), Geometry::sphere(attributes, planet->innerRadius, 100, 50), fromAtmosphere);
//  node->setRotation(QVector3D(-90,0,180));
}
void Sun::draw(){
  checkMaterialToggle();
  float time = float(Timer::Instance().secoundsPassed) + float(Timer::Instance().nanosecoundsPassed)/1000000000.0;
  node->getMaterial()->getShaderProgram()->use();
  node->getMaterial()->getShaderProgram()->setUniform("time", time);
  SceneData::Instance().getCurrentCamera()->setUniforms(
      node->getMaterial()->getShaderProgram(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());
  node->draw();
}
