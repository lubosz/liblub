/*
 * Atmosphere.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include <math.h>
#include "Atmosphere.h"
#include "Material/Materials.h"

Atmosphere::Atmosphere() {
  // TODO Auto-generated constructor stub

}

Atmosphere::~Atmosphere() {
  // TODO Auto-generated destructor stub
}

void Atmosphere::setAtmoUniforms(ShaderProgram * program, float innerRadius, float outerRadius) {

   float wavelength[3];
   wavelength[0] = 0.650f; // 650 nm for red
//    wavelength[1] = 0.570f; // 570 nm for green
//    wavelength[2] = 0.475f; // 475 nm for blue

   wavelength[1] = 1.0; // 570 nm for green
   wavelength[2] = 0.0; // 475 nm for blue

   float wavelength4[3];
   wavelength4[0] = powf(wavelength[0], 4.0f);
   wavelength4[1] = powf(wavelength[1], 4.0f);
   wavelength4[2] = powf(wavelength[2], 4.0f);

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
   program->setUniform("invWavelength", QVector3D(1 / wavelength4[0], 1 / wavelength4[1], 1 / wavelength4[2]));
   program->setUniform("innerRadius", innerRadius);
   program->setUniform("innerRadius2", innerRadius * innerRadius);
   program->setUniform("outerRadius", outerRadius);
   program->setUniform("outerRadius2", outerRadius * outerRadius);
   program->setUniform("rayleighSun", rayleigh * sun);
   program->setUniform("mieSun", mie * sun);
   program->setUniform("rayleigh4Pi", rayleigh4Pi);
   program->setUniform("mie4Pi", mie4Pi);
   program->setUniform("invSphereDistance", 1.0f / (outerRadius - innerRadius));
   program->setUniform("scaleDepth", rayleighScaleDepth);
   program->setUniform("scaleOverScaleDepth", (1.0f / (outerRadius - innerRadius)) / rayleighScaleDepth);
   program->setUniform("g", g);
   program->setUniform("g2", g * g);
 }

void Atmosphere::init() {
  skyFromAtmosphere = new Template("Atmo/Sky",attributes);
  TemplateEngine::Instance().c.insert("fromSpace", true);
  skyFromSpace = new Template("Atmo/Sky",attributes);
  skyNode = new Node("sky", { 0, 0, 0 }, 1, outerSphere, skyFromAtmosphere);
  setAtmoUniforms(skyFromAtmosphere->getShaderProgram());
  setAtmoUniforms(skyFromSpace->getShaderProgram());
}

void Atmosphere::draw(){
  if(camera->position.length() >= outerRadius)
      skyNode->setMaterial(skyFromSpace);
    else
      skyNode->setMaterial(skyFromAtmosphere);

    setCameraUniforms(skyNode->getMaterial()->getShaderProgram());
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);

    glBlendFunc(GL_ONE, GL_ONE);
    skyNode->setView(camera);
    skyNode->draw();

    glDisable(GL_BLEND);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}
