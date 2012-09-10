/*
 * PlaneMoon.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef PLANEMOON_H_
#define PLANEMOON_H_

#include "Scene/Node.h"
#include "Mesh/Mesh.h"
#include "PlanetElement.h"

class PlaneMoon  : public PlanetElement{
public:
  PlaneMoon(Planet * planet);
  void init();
  void draw();
  virtual ~PlaneMoon();
  Mesh * moonPlane(const QList<string> & attributes);
};

#endif /* PLANEMOON_H_ */
