/*
 * Atmosphere.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef ATMOSPHERE_H_
#define ATMOSPHERE_H_

#include "Scene/Node.h"
#include "Material/Material.h"
#include "PlanetElement.h"

class Atmosphere  : public PlanetElement{
public:
  Node * skyNode;
  Material *skyFromAtmosphere, *skyFromSpace;
  Atmosphere(float innerRadius, float outerRadius);
  virtual ~Atmosphere();
  void init();
  void draw();
  static void setAtmoUniforms(ShaderProgram * program, float innerRadius, float outerRadius);
};

#endif /* ATMOSPHERE_H_ */
