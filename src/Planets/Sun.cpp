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
  QList<string> sunAttributes;
  sunAttributes.push_back("normal");
  sunAttributes.push_back("uv");
  Material * sunMaterial = new Template("perlin",sunAttributes);
  node = new Node("sun", planet->position, planet->getSize(), Geometry::sphere(sunAttributes, planet->innerRadius, 100, 50), sunMaterial);
  node->setRotation(QVector3D(-90,0,180));
  perlinNoise = sunMaterial->getShaderProgram();
}
void Sun::draw(){
  float time = float(Timer::Instance().secoundsPassed) + float(Timer::Instance().nanosecoundsPassed)/1000000000.0;
  perlinNoise->use();
  perlinNoise->setUniform("time", time);
  node->setView(SceneData::Instance().getCurrentCamera());
  node->draw();
}
