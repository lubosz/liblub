/*
 * Ground.h
 *
 *  Created on: May 6, 2011
 *      Author: bmonkey
 */

#ifndef PLANETELEMENT_H_
#define PLANETELEMENT_H_

#include <QVector3D>

class Planet;

class PlanetElement {
public:
  Planet * planet;
  PlanetElement() {};
  PlanetElement(Planet * planet);
  virtual ~PlanetElement();
  virtual void init() = 0;
  virtual void draw() = 0;
};

#endif /* PlanetElement */
