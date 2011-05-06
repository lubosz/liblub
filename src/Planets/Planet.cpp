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

#include "Terrain.h"
#include "Ocean.h"
#include "Sun.h"
//#include "PlaneMoon.h"

Planet::Planet(float innerRadius, float outerRadius, PlanetType type, const QVector3D atmoColor) {
  switch (type){
  case sun:
    ground = new Sun(innerRadius,outerRadius);
    break;
  case terrain:
    ground = new Terrain(innerRadius,outerRadius, atmoColor);
    break;
  case ocean:
    ground = new Ocean(innerRadius,outerRadius);
    break;
  }
  atmoSphere = new Atmosphere(innerRadius,outerRadius, atmoColor);
}

Planet::~Planet() {
}

void Planet::init(const QVector3D& position, float size) {
  atmoSphere->init(position, size);
  ground->init(position, size);
}

void Planet::draw() {
  ground->draw();
//  atmoSphere->draw();
}
