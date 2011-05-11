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
}

Planet::~Planet() {
  attenuation = true;
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

void Planet::setAttenuation(bool attenuation) {
  this->attenuation = attenuation;
  atmoSphere->updateAttenuation();
  foreach(PlanetElement * element, elements)
    element->updateAttenuation();
}

void Planet::setRed(int red){
  lightWavelength.setX(float(red)/256.0);
  updateWaveLength();
}

void Planet::setGreen(int green){
  lightWavelength.setY(float(green)/256.0);
  updateWaveLength();
}

void Planet::setBlue(int blue){
  lightWavelength.setZ(float(blue)/256.0);
  updateWaveLength();
}
