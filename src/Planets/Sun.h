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

class Sun {
public:
  ShaderProgram * perlinNoise;
  Node *sunNode;
  Sun();
  virtual ~Sun();
  void init();
  void draw();
};

#endif /* SUN_H_ */
