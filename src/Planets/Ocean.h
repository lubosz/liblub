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

class Ocean {
public:
  Node * oceanNode;
  Material * ocean;
  Ocean();
  virtual ~Ocean();
  void init(float innerRadius);
  void draw();
};

#endif /* OCEAN_H_ */
