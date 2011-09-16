/*
 * Terrain.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef TERRAINTESSELATION_H_
#define TERRAINTESSELATION_H_

#include "Scene/Node.h"
#include "PlanetElement.h"

class TerrainTesselation  : public PlanetElement{
public:
  ShaderProgram *shader;
  TerrainTesselation(Planet * planet);
  virtual ~TerrainTesselation();
  void init();
  void draw();
  void updateTesselation();
};

#endif /* TERRAINTESSELATION_H_ */
