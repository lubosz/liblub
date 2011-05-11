/*
 * Ground.h
 *
 *  Created on: May 6, 2011
 *      Author: bmonkey
 */

#ifndef PLANETELEMENT_H_
#define PLANETELEMENT_H_

#include <QVector3D>
#include "Material/ShaderProgram.h"
#include "Scene/Node.h"
class Planet;

class PlanetElement {
public:
  Planet * planet;
  Node * node;
  bool attenuation;
  PlanetElement() {};
  PlanetElement(Planet * planet);
  virtual ~PlanetElement();
  virtual void init() = 0;
  virtual void draw() = 0;
  void updateWaveLength();
  void updateWaveLength(ShaderProgram * program);
  void setAttenuation(bool attenuation);
  void updateAttenuation();
protected:
  void setAtmoUniforms(ShaderProgram * program);
};

#endif /* PlanetElement */
