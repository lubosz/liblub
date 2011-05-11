/*
 * Planet.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include <string>
#include <QList>
#include "Planet.h"
#include "Mesh/Geometry.h"

#include "TerrainPlain.h"
#include "TerrainTesselation.h"
#include "Ocean.h"
#include "Sun.h"
//#include "PlaneMoon.h"

float Planet::getSize() const {
    return size;
}

Planet::Planet(float innerRadius, float outerRadius, PlanetType type, const QVector3D & lightWavelength, const QVector3D & position, float size)
: innerRadius(innerRadius),outerRadius(outerRadius),type(type),lightWavelength(lightWavelength),position(position),size(size){
  useAttenuation = true;
  useRayleigh = true;
  useMie = true;
}

Planet::~Planet() {
}

void Planet::init() {
  switch (type){
  case sun:
    elements.push_back(new Sun(this));
    break;
  case terrainPlain:
    elements.push_back(new TerrainPlain(this));
    break;
  case terrainTess:
    elements.push_back(new TerrainTesselation(this));
    break;
  case ocean:
    elements.push_back(new Ocean(this));
    break;
  }
  atmoSphere = new Atmosphere(this);
  foreach(PlanetElement * element, elements)
    element->init();
  atmoSphere->init();
}

void Planet::draw() {
  foreach(PlanetElement * element, elements)
    element->draw();
}

void Planet::setWaveLength(const QVector3D & lightWavelength) {
  this->lightWavelength = lightWavelength;
}

void Planet::updateWaveLength(){
  atmoSphere->updateWaveLength();
  foreach(PlanetElement * element, elements)
    element->updateWaveLength();
}

void Planet::updatePosition(){
  atmoSphere->node->setPosition(position);
  foreach(PlanetElement * element, elements)
    element->node->setPosition(position);
}

void Planet::updateScattering(){
  atmoSphere->updateUseMie();
  atmoSphere->updateUseRayleigh();
  foreach(PlanetElement * element, elements){
    element->updateUseMie();
    element->updateUseRayleigh();
  }
}

void Planet::setAttenuation(bool attenuation) {
  this->useAttenuation = attenuation;
  atmoSphere->updateUseAttenuation();
  foreach(PlanetElement * element, elements)
    element->updateUseAttenuation();
}

void Planet::setRed(double red){
  lightWavelength.setX(red);
  updateWaveLength();
}

void Planet::setGreen(double green){
  lightWavelength.setY(green);
  updateWaveLength();
}

void Planet::setBlue(double blue){
  lightWavelength.setZ(blue);
  updateWaveLength();
}

void Planet::setX(double x){
  position.setX(x);
  updatePosition();
}

void Planet::setY(double y){
  position.setY(y);
  updatePosition();
}

void Planet::setZ(double z){
  position.setZ(z);
  updatePosition();
}

void Planet::setMie(bool mie){
  useMie = mie;
  updateScattering();
}
void Planet::setRayleigh(bool rayleigh){
  useRayleigh = rayleigh;
  updateScattering();
}
