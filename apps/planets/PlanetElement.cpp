/*
 * Ground.cpp
 *
 *  Created on: May 6, 2011
 *      Author: bmonkey
 */

#include "PlanetElement.h"
#include "Planet.h"
#include "Scene/Scene.h"
#include "System/TemplateEngine.h"
#include "Material/Shaders.h"

PlanetElement::PlanetElement(Planet * planet) : planet(planet) {
}

PlanetElement::~PlanetElement() {
  // TODO Auto-generated destructor stub
}

void PlanetElement::initMaterials(string name, const QList<string> & attributes){
  TemplateEngine::Instance().c.insert("fromSpace", QVariant(false));
  fromAtmosphere = new VertFragProgram(name,attributes);
  TemplateEngine::Instance().c.insert("fromSpace", QVariant(true));
  fromSpace = new VertFragProgram(name,attributes);
  setAtmoUniforms(fromAtmosphere);
  setAtmoUniforms(fromSpace);
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
  updateWaveLength(node->getShader());
}

void PlanetElement::updateUseAttenuation(){
  ShaderProgram * program = node->getShader();
  program->use();
  program->setUniform("attenuation", planet->useAttenuation);
}

void PlanetElement::updateUseMie(){
  ShaderProgram * program = node->getShader();
  program->use();
  program->setUniform("useMie", planet->useMie);
}

void PlanetElement::updateUseRayleigh(){
  ShaderProgram * program = node->getShader();
  program->use();
  program->setUniform("useRayleigh", planet->useRayleigh);
}

void PlanetElement::updateSize(){
  ShaderProgram * program = node->getShader();
  program->use();
  program->setUniform("invSphereDistance",
      1.0f / (planet->outerRadius - planet->innerRadius));
  program->setUniform("scaleDepth", planet->rayleighScaleDepth);
  program->setUniform("scaleOverScaleDepth",
      (1.0f / (planet->outerRadius - planet->innerRadius)) / planet->rayleighScaleDepth);
  program->setUniform("innerRadius", planet->innerRadius);
  program->setUniform("innerRadius2", planet->innerRadius * planet->innerRadius);
  program->setUniform("outerRadius", planet->outerRadius);
  program->setUniform("outerRadius2", planet->outerRadius * planet->outerRadius);
}

void PlanetElement::setAtmoUniforms(ShaderProgram * program) {
  updateWaveLength(program);

  QVector3D lightPosition = Scene::Instance().getLight("sunlight")->position;
  QVector3D lightDirection = lightPosition - planet->position;
  lightDirection.normalize();

  float rayleigh = 0.0025f; // Rayleigh scattering constant
  float rayleigh4Pi = rayleigh * 4.0f * M_PI;
  float mie = 0.0010f; // Mie scattering constant
  float mie4Pi = mie * 4.0f * M_PI;
  float sun = 20.0f; // Sun brightness constant
  float g = -0.990f; // The Mie phase asymmetry factor

  //    float mieScaleDepth = 0.1f;

  program->use();
  program->setUniform("lightPosition", lightDirection);

  program->setUniform("rayleighSun", rayleigh * sun);
  program->setUniform("mieSun", mie * sun);
  program->setUniform("rayleigh4Pi", rayleigh4Pi);
  program->setUniform("mie4Pi", mie4Pi);

  program->setUniform("g", g);
  program->setUniform("g2", g * g);
  program->setUniform("useRayleigh", planet->useRayleigh);
  program->setUniform("useMie", planet->useMie);
  program->setUniform("attenuation", planet->useAttenuation);
}

void  PlanetElement::checkMaterialToggle(){
  QVector3D camFromPlanet = Scene::Instance().getCurrentCamera()->position
      - planet->position;
  float camDistance = camFromPlanet.length();
  if (camDistance >= planet->outerRadius) {
    node->setShader(fromSpace);
    updateWaveLength();
    updateUseAttenuation();
    updateSize();
  } else {
    node->setShader(fromAtmosphere);
    updateWaveLength();
    updateUseAttenuation();
    updateSize();
  }
}
