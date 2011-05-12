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

void Ocean::setOceanUniforms(ShaderProgram * program){
  program->use();
  program->setUniform("deepColor", QVector4D(0, 0.3, 0.5, 1.0));
  program->setUniform("shallowColor",QVector4D(0, 1, 1, 1.0));
  program->setUniform("reflectionColor",QVector4D(0.95, 1, 1, 1.0));
  program->setUniform("reflectionAmount",1.0f);
  program->setUniform("reflectionBlur",0.0f);
  program->setUniform("waterAmount",0.3f);
  program->setUniform("fresnelPower",5.0f);
  program->setUniform("fresnelBias",0.328f);
  program->setUniform("hdrMultiplier", 0.471f);
  program->setUniform("textureScale",QVector2D(25, 26));
  program->setUniform("bumpSpeed",QVector2D(0.015, 0.005));
  program->setUniform("BumpScale",0.2f);
  program->setUniform("time",0.7f);
  program->setUniform("waveFreq",0.028f);
  program->setUniform("waveAmp",1.8f);
}

void Ocean::init(){
  QList<string> attributes = QList<string>() << "normal" << "uv";

  initMaterials("Ocean", attributes);
  setOceanUniforms(fromAtmosphere->getShaderProgram());
  setOceanUniforms(fromSpace->getShaderProgram());

  Texture * oceanNormal = new TextureFile("ocean/waves.png","NormalMap");
  fromAtmosphere->addTexture(oceanNormal);
  fromSpace->addTexture(oceanNormal);
  Texture * oceanSky = new CubeTextureFile("cubemaps/sky","EnvironmentMap");
  fromAtmosphere->addTexture(oceanSky);
  fromSpace->addTexture(oceanSky);

  Mesh * innerSphere = Geometry::sphere(attributes, planet->innerRadius, 100, 50);
  node = new Node("ocean", planet->position, planet->getSize(), innerSphere, ocean);

}
void Ocean::draw() {
  checkMaterialToggle();

  node->setView(SceneData::Instance().getCurrentCamera());
//    ocean->getShaderProgram()->setUniform("time",time);
  node->getMaterial()->getShaderProgram()->setUniform("eyePosition",SceneData::Instance().getCurrentCamera()->position);

  SceneData::Instance().getCurrentCamera()->setUniforms(
      node->getMaterial()->getShaderProgram(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());

  node->draw();
}
