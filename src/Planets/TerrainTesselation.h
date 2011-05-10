/*
 * Terrain.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef TERRAINTESSELATION_H_
#define TERRAINTESSELATION_H_

#include "Scene/Node.h"
#include "Material/Material.h"
#include "PlanetElement.h"

class TerrainTesselation  : public PlanetElement{
public:
  Node *terrainNode;
  Material *terrainMat;
  TerrainTesselation(Planet * planet);
  virtual ~TerrainTesselation();
  void init();
  void draw();
  void updateTesselation();
};

#endif /* TERRAINTESSELATION_H_ */
