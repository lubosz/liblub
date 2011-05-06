/*
 * Ground.h
 *
 *  Created on: May 6, 2011
 *      Author: bmonkey
 */

#ifndef PLANETELEMENT_H_
#define PLANETELEMENT_H_

#include <QVector3D>

class PlanetElement {
public:
  float innerRadius, outerRadius;
  QVector3D position;
  PlanetElement() {};
  PlanetElement(float innerRadius, float outerRadius);
  virtual ~PlanetElement();
  virtual void init(const QVector3D& position, float size) = 0;
  virtual void draw() = 0;
};

#endif /* PlanetElement */
