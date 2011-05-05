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

class Terrain {
public:
  Node * groundNode, *terrainNode;
  Material *terrainMat, *groundFromAtmosphere, *groundFromSpace;
  Terrain();
  virtual ~Terrain();
  void init(float innerRadius);
  void draw();
  void updateTesselation();
};

#endif /* TERRAIN_H_ */
