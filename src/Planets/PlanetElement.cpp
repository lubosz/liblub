/*
 * Ground.cpp
 *
 *  Created on: May 6, 2011
 *      Author: bmonkey
 */

#include "PlanetElement.h"
#include "Planet.h"

PlanetElement::PlanetElement(Planet * planet) : planet(planet) {
}

PlanetElement::~PlanetElement() {
  // TODO Auto-generated destructor stub
}

void PlanetElement::updateWaveLength(ShaderProgram * program){
  float wavelength[3];
  wavelength[0] = planet->lightWavelength.x();
  wavelength[1] = planet->lightWavelength.y();
  wavelength[2] = planet->lightWavelength.z();

  float wavelength4[3];
  wavelength4[0] = powf(wavelength[0], 4.0f);
  wavelength4[1] = powf(wavelength[1], 4.0f);
  wavelength4[2] = powf(wavelength[2], 4.0f);

  program->use();
  program->setUniform("invWavelength", QVector3D(1 / wavelength4[0], 1 / wavelength4[1], 1 / wavelength4[2]));
}

void PlanetElement::updateWaveLength(){
  updateWaveLength(node->getMaterial()->getShaderProgram());
}

void PlanetElement::setAtmoUniforms(ShaderProgram * program) {
  updateWaveLength(program);

  QVector3D lightPosition = QVector3D(0, 0, 1000);
  QVector3D lightDirection = lightPosition / lightPosition.length();

  float rayleigh = 0.0025f; // Rayleigh scattering constant
  float rayleigh4Pi = rayleigh * 4.0f * M_PI;
  float mie = 0.0010f; // Mie scattering constant
  float mie4Pi = mie * 4.0f * M_PI;
  float sun = 20.0f; // Sun brightness constant
  float g = -0.990f; // The Mie phase asymmetry factor
  float rayleighScaleDepth = 0.25f;
  //    float mieScaleDepth = 0.1f;

  program->use();
  program->setUniform("lightPosition", lightDirection);
  program->setUniform("innerRadius", planet->innerRadius);
  program->setUniform("innerRadius2", planet->innerRadius * planet->innerRadius);
  program->setUniform("outerRadius", planet->outerRadius);
  program->setUniform("outerRadius2", planet->outerRadius * planet->outerRadius);
  program->setUniform("rayleighSun", rayleigh * sun);
  program->setUniform("mieSun", mie * sun);
  program->setUniform("rayleigh4Pi", rayleigh4Pi);
  program->setUniform("mie4Pi", mie4Pi);
  program->setUniform("invSphereDistance",
      1.0f / (planet->outerRadius - planet->innerRadius));
  program->setUniform("scaleDepth", rayleighScaleDepth);
  program->setUniform("scaleOverScaleDepth",
      (1.0f / (planet->outerRadius - planet->innerRadius)) / rayleighScaleDepth);
  program->setUniform("g", g);
  program->setUniform("g2", g * g);
}
