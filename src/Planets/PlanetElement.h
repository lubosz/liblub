/*
 * Ground.h
 *
 *  Created on: May 6, 2011
 *      Author: bmonkey
 */

#ifndef PLANETELEMENT_H_
#define PLANETELEMENT_H_

class PlanetElement {
public:
  float innerRadius, outerRadius;
  PlanetElement() {};
  PlanetElement(float innerRadius, float outerRadius);
  virtual ~PlanetElement();
  virtual void init() = 0;
  virtual void draw() = 0;
};

#endif /* PlanetElement */
