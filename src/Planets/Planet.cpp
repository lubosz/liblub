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
  terrain = new Terrain();
  ocean = new Ocean();
  atmoSphere = new Atmosphere();
  sun = new Sun();
  planeMoon = new PlaneMoon();

}

Planet::~Planet() {
  // TODO Auto-generated destructor stub
}

void Planet::init() {

  QList<string> attributes;
  attributes.push_back("normal");
  attributes.push_back("uv");

//
//  Mesh * outerSphere = Geometry::sphere(attributes, outerRadius, 300, 500);

  terrain->init();
  ocean->init();
  atmoSphere->init();
  sun->init();
  planeMoon->init();

}

void Planet::draw() {
//    drawMoon();
//    drawGround();
//  drawOcean();
//    drawSun();
//  drawSky();
  terrain->draw();
  ocean->draw();
  atmoSphere->draw();
  sun->draw();
  planeMoon->draw();
}
