/*
 * Planet.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef PLANET_H_
#define PLANET_H_

#include <vector>
#include "PlanetElement.h"
#include "Atmosphere.h"

using std::vector;

class Planet: public QObject {
  Q_OBJECT
public:
  enum PlanetType {
    sun = 0,
    ocean,
    terrainPlain,
    terrainTess
  };
  float innerRadius;
  float outerRadius;
  PlanetType type;
  QVector3D lightWavelength;
  QVector3D position;
  vector<PlanetElement*> elements;
  Atmosphere * atmoSphere;
  bool attenuation;

  Planet(float innerRadius, float outerRadius, PlanetType type, const QVector3D & lightWavelength, const QVector3D & position, float size);
  virtual ~Planet();
  void init();
  void draw();
  float getSize() const;
  void updateWaveLength();
  void setWaveLength(const QVector3D & lightWavelength);
private:
  float size;

public slots:
  void setRed(double red);
  void setGreen(double red);
  void setBlue(double red);
  void setAttenuation(bool attenuation);
};

#endif /* PLANET_H_ */
