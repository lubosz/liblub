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
  QVector3D atmoColor;
  Node * groundNode;
  Material *groundFromAtmosphere, *groundFromSpace;
  TerrainPlain(float innerRadius, float outerRadius, const QVector3D atmoColor);
  virtual ~TerrainPlain();
  void init(const QVector3D& position, float size);
  void draw();
};

#endif /* TERRAINPLAIN_H_ */
