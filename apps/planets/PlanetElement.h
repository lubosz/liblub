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
  ShaderProgram *fromAtmosphere, *fromSpace;
  PlanetElement() {}
  PlanetElement(Planet * planet);
  virtual ~PlanetElement();
  virtual void init() = 0;
  virtual void draw() = 0;
  void updateWaveLength();
  void updateUseMie();
  void updateUseRayleigh();
  void updateSize();
  void updateWaveLength(ShaderProgram * program);
  void updateUseAttenuation();
  void initMaterials(string name, const QList<string> & attributes);
  void checkMaterialToggle();
protected:
  void setAtmoUniforms(ShaderProgram * program);
};

#endif /* PlanetElement */
