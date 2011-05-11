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
#include "Planet.h"

TerrainPlain::TerrainPlain(Planet * planet){
  this->planet = planet;
}

TerrainPlain::~TerrainPlain() {
  // TODO Auto-generated destructor stub
}

void TerrainPlain::init(){
   QList<string> attributes;
    attributes.push_back("normal");
    attributes.push_back("uv");

//  Texture * earthMap = new TextureFile("earthmap1k.jpg",
//      "planet");
    Texture * earthMap = new TextureFile("Planets/Mars.jpg",
        "planet");
  groundFromAtmosphere = new Template("Atmo/Ground",attributes);
  groundFromAtmosphere->addTexture(earthMap);
  TemplateEngine::Instance().c.insert("fromSpace", true);
  groundFromSpace = new Template("Atmo/Ground",attributes);
  groundFromSpace->addTexture(earthMap);

  Mesh * innerSphere = Geometry::sphere(attributes, planet->innerRadius, 100, 50);
  node = new Node("ground", planet->position, planet->getSize(), innerSphere, groundFromAtmosphere);
  node->setRotation(QVector3D(-90, 0, 0));
  setAtmoUniforms(groundFromAtmosphere->getShaderProgram());
  setAtmoUniforms(groundFromSpace->getShaderProgram());
 }

 void TerrainPlain::draw() {
  QVector3D camFromPlanet = SceneData::Instance().getCurrentCamera()->position
      - planet->position;
  float camDistance = camFromPlanet.length();
  if (camDistance >= planet->outerRadius) {

    node->setMaterial(groundFromSpace);
    updateWaveLength();
  } else {
    node->setMaterial(groundFromAtmosphere);
    updateWaveLength();

  }

  SceneData::Instance().getCurrentCamera()->setUniforms(
      node->getMaterial()->getShaderProgram(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());

  node->draw();
}
