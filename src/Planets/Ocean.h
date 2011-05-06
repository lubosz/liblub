/*
 * Ocean.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef OCEAN_H_
#define OCEAN_H_

#include "Scene/Node.h"
#include "Material/Material.h"
#include "PlanetElement.h"

class Ocean : public PlanetElement{
public:
  Node * oceanNode;
  Material * ocean;
  Ocean(float innerRadius, float outerRadius);
  virtual ~Ocean();
  void init(const QVector3D& position, float size);
  void draw();
};

#endif /* OCEAN_H_ */
