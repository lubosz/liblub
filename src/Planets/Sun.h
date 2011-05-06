/*
 * Sun.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef SUN_H_
#define SUN_H_

#include "Scene/Node.h"
#include "Material/ShaderProgram.h"

#include "PlanetElement.h"

class Sun: public PlanetElement {
public:
  ShaderProgram * perlinNoise;
  Node *sunNode;
  Sun(float innerRadius, float outerRadius);
  virtual ~Sun();
  void init(const QVector3D& position, float size);
  void draw();
};

#endif /* SUN_H_ */
