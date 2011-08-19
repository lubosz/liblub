/*
 * Terrain.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef TERRAINPLAIN_H_
#define TERRAINPLAIN_H_

#include "Scene/Node.h"
#include "Material/Material.h"
#include "PlanetElement.h"

class TerrainPlain  : public PlanetElement{
public:
  TerrainPlain(Planet * planet);
  virtual ~TerrainPlain();
  void init();
  void draw();
};

#endif /* TERRAINPLAIN_H_ */
