/*
 * Planet.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef PLANET_H_
#define PLANET_H_

#include "Atmosphere.h"
#include "PlanetElement.h"


class Planet {
public:
  enum PlanetType {
    sun = 0,
    ocean,
    terrain
  };
  Atmosphere * atmoSphere;
  PlanetElement * ground;
//  Terrain * terrain;
//  Ocean * ocean;
//  Sun * sun;
//  PlaneMoon * planeMoon;
  Planet(float innerRadius, float outerRadius, PlanetType type);
  virtual ~Planet();
  void init(const QVector3D& position, float size);
  void draw();
};

#endif /* PLANET_H_ */
