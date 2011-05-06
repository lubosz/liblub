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
  QVector3D atmoColor;
  Node *terrainNode;
  Material *terrainMat;
  TerrainTesselation(float innerRadius, float outerRadius, const QVector3D atmoColor);
  virtual ~TerrainTesselation();
  void init(const QVector3D& position, float size);
  void draw();
  void updateTesselation();
};

#endif /* TERRAINTESSELATION_H_ */
