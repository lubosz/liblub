/*
 * Ocean.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "Ocean.h"
#include "Material/Materials.h"
#include "Material/Textures.h"
#include "Mesh/Geometry.h"
#include "Scene/SceneData.h"
#include "Planet.h"

Ocean::Ocean(Planet * planet) {
  this->planet = planet;
}

Ocean::~Ocean() {
  // TODO Auto-generated destructor stub
}

void Ocean::init(){
  QList<string> attributes;
   attributes.push_back("normal");
   attributes.push_back("uv");

  ocean = new Template("Ocean",attributes);
  ocean->getShaderProgram()->use();
  ocean->getShaderProgram()->setUniform("deepColor", QVector4D(0, 0.3, 0.5, 1.0));
  ocean->getShaderProgram()->setUniform("shallowColor",QVector4D(0, 1, 1, 1.0));
  ocean->getShaderProgram()->setUniform("reflectionColor",QVector4D(0.95, 1, 1, 1.0));
  ocean->getShaderProgram()->setUniform("reflectionAmount",1.0f);
  ocean->getShaderProgram()->setUniform("reflectionBlur",0.0f);
  ocean->getShaderProgram()->setUniform("waterAmount",0.3f);
  ocean->getShaderProgram()->setUniform("fresnelPower",5.0f);
  ocean->getShaderProgram()->setUniform("fresnelBias",0.328f);
  ocean->getShaderProgram()->setUniform("hdrMultiplier", 0.471f);
  ocean->getShaderProgram()->setUniform("textureScale",QVector2D(25, 26));
  ocean->getShaderProgram()->setUniform("bumpSpeed",QVector2D(0.015, 0.005));
  ocean->getShaderProgram()->setUniform("BumpScale",0.2f);
  ocean->getShaderProgram()->setUniform("time",0.7f);
  ocean->getShaderProgram()->setUniform("waveFreq",0.028f);
  ocean->getShaderProgram()->setUniform("waveAmp",1.8f);

  Texture * oceanNormal = new TextureFile("ocean/waves.png","NormalMap");
  ocean->addTexture(oceanNormal);
  Texture * oceanSkyMap = new TextureFile("Planets/TychoSkymap.jpg","skyMap");
  ocean->addTexture(oceanSkyMap);
  Texture * oceanSky = new CubeTextureFile("cubemaps/sky","EnvironmentMap");
  ocean->addTexture(oceanSky);
  Mesh * innerSphere = Geometry::sphere(attributes, planet->innerRadius, 100, 50);
  node = new Node("ocean", planet->position, planet->getSize(), innerSphere, ocean);

  setAtmoUniforms(ocean->getShaderProgram());
}
void Ocean::draw() {
  node->setView(SceneData::Instance().getCurrentCamera());
//    ocean->getShaderProgram()->setUniform("time",time);
  ocean->getShaderProgram()->setUniform("eyePosition",SceneData::Instance().getCurrentCamera()->position);

  SceneData::Instance().getCurrentCamera()->setUniforms(
      node->getMaterial()->getShaderProgram(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());

  node->draw();
}
