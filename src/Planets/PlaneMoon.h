/*
 * PlaneMoon.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef PLANEMOON_H_
#define PLANEMOON_H_

#include "Scene/Node.h"
#include "Material/Material.h"
#include "Mesh/Mesh.h"

class PlaneMoon {
public:
  Material *spaceFromAtmosphere, *spaceFromSpace;
  Node * spaceNode;
  PlaneMoon();
  void init();
  void draw();
  virtual ~PlaneMoon();
  Mesh * moonPlane(const QList<string> & attributes);
};

#endif /* PLANEMOON_H_ */
