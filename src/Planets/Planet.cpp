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


Planet::Planet() {
  innerRadius = 11.0f;
  outerRadius = 11.55f;
  terrain = new Terrain(innerRadius,outerRadius);
  ocean = new Ocean(innerRadius,outerRadius);
  atmoSphere = new Atmosphere(innerRadius,outerRadius);
  sun = new Sun(innerRadius,outerRadius);
  planeMoon = new PlaneMoon(innerRadius,outerRadius);

}

Planet::~Planet() {
  // TODO Auto-generated destructor stub
}

void Planet::init() {

  QList<string> attributes;
  attributes.push_back("normal");
  attributes.push_back("uv");

  terrain->init();
  ocean->init();
  atmoSphere->init();
  sun->init();
  planeMoon->init();

}

void Planet::draw() {
//  terrain->draw(outerRadius);
  ocean->draw();
//  sun->draw();
  atmoSphere->draw();
//  planeMoon->draw(outerRadius);
}
