/*
 * Terrain.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "Scene/Node.h"
#include "Material/Material.h"
#include "PlanetElement.h"

class Terrain  : public PlanetElement{
public:
  Node * groundNode, *terrainNode;
  Material *terrainMat, *groundFromAtmosphere, *groundFromSpace;
  Terrain(float innerRadius, float outerRadius);
  virtual ~Terrain();
  void init();
  void draw();
  void updateTesselation();
};

#endif /* TERRAIN_H_ */
