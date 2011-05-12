/*
 * Terrain.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "TerrainPlain.h"
#include "Material/Textures.h"

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
    QList<string> attributes = QList<string>() << "normal" << "uv";
    initMaterials("Atmo/Ground", attributes);
//  Texture * earthMap = new TextureFile("earthmap1k.jpg",
//      "planet");
    Texture * earthMap = new TextureFile("Planets/Mars.jpg",
        "planet");
    fromAtmosphere->addTexture(earthMap);
    fromSpace->addTexture(earthMap);
  Mesh * innerSphere = Geometry::sphere(attributes, planet->innerRadius, 100, 50);
  node = new Node("ground", planet->position, planet->getSize(), innerSphere, fromAtmosphere);
  node->setRotation(QVector3D(-90, 0, 0));
 }

 void TerrainPlain::draw() {
   checkMaterialToggle();

  SceneData::Instance().getCurrentCamera()->setUniforms(
      node->getMaterial()->getShaderProgram(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());

  node->draw();
}
