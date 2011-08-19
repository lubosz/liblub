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
  Atmosphere(Planet * planet);
  virtual ~Atmosphere();
  void init();
  void draw();
};

#endif /* ATMOSPHERE_H_ */
