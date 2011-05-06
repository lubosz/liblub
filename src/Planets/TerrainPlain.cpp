/*
 * Terrain.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "TerrainPlain.h"
#include "Material/Textures.h"
#include "Material/Materials.h"
#include "System/TemplateEngine.h"
#include "System/GUI.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Atmosphere.h"
#include "Scene/SceneData.h"

TerrainPlain::TerrainPlain(float innerRadius, float outerRadius, const QVector3D atmoColor) : atmoColor(atmoColor){
  this->innerRadius = innerRadius;
  this->outerRadius = outerRadius;
}

TerrainPlain::~TerrainPlain() {
  // TODO Auto-generated destructor stub
}

void TerrainPlain::init(const QVector3D& position, float size){
   QList<string> attributes;
    attributes.push_back("normal");
    attributes.push_back("uv");
    this->position = position;

  Texture * earthMap = new TextureFile("earthmap1k.jpg",
      "planet");
  groundFromAtmosphere = new Template("Atmo/Ground",attributes);
  groundFromAtmosphere->addTexture(earthMap);
  TemplateEngine::Instance().c.insert("fromSpace", true);
  groundFromSpace = new Template("Atmo/Ground",attributes);
  groundFromSpace->addTexture(earthMap);

  Mesh * innerSphere = Geometry::sphere(attributes, innerRadius, 100, 50);
  groundNode = new Node("ground", position, size, innerSphere, groundFromAtmosphere);
  groundNode->setRotation(QVector3D(-90, 0, 0));
  Atmosphere::setAtmoUniforms(groundFromAtmosphere->getShaderProgram(),innerRadius, outerRadius, atmoColor);
  Atmosphere::setAtmoUniforms(groundFromSpace->getShaderProgram(),innerRadius, outerRadius, atmoColor);
 }

 void TerrainPlain::draw() {
  QVector3D camFromPlanet = SceneData::Instance().getCurrentCamera()->position - position;
  float camDistance = camFromPlanet.length();
  if (camDistance >= outerRadius)
    groundNode->setMaterial(groundFromSpace);
  else
    groundNode->setMaterial(groundFromAtmosphere);

  SceneData::Instance().getCurrentCamera()->setUniforms(groundNode->getMaterial()->getShaderProgram(), position);
  groundNode->setView(SceneData::Instance().getCurrentCamera());

  groundNode->draw();
}
