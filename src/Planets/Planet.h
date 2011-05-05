/*
 * Planet.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef PLANET_H_
#define PLANET_H_

#include "Terrain.h"
#include "Ocean.h"
#include "Atmosphere.h"
#include "Sun.h"
#include "PlaneMoon.h"

class Planet {
public:
  float innerRadius;
  float outerRadius;
  Terrain * terrain;
  Ocean * ocean;
  Atmosphere * atmoSphere;
  Sun * sun;
  PlaneMoon * planeMoon;
  Planet();
  virtual ~Planet();
  void init();
  void draw();
};

#endif /* PLANET_H_ */
