/*
 * Terrain.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "TerrainTesselation.h"
#include "Material/Textures.h"
#include "Material/Materials.h"
#include "System/TemplateEngine.h"
#include "System/GUI.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Atmosphere.h"
#include "Scene/SceneData.h"
#include "Planet.h"

TerrainTesselation::TerrainTesselation(Planet * planet) {
  this->planet = planet;
}

TerrainTesselation::~TerrainTesselation() {
  // TODO Auto-generated destructor stub
}

void TerrainTesselation::init(){
   QList<string> attributes;
    attributes.push_back("normal");
    attributes.push_back("uv");

   terrainMat = new EmptyMat();

   terrainMat->init();
//    terrainMat->getShaderProgram()->attachShader("Atmo/GroundTesselation.vert",GL_VERTEX_SHADER,true);
   terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.vert",GL_VERTEX_SHADER,true);
   terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.eval",GL_TESS_EVALUATION_SHADER,true);
   terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.cont",GL_TESS_CONTROL_SHADER,true);
   terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.geom",GL_GEOMETRY_SHADER,true);
//    terrainMat->getShaderProgram()->attachShader("Atmo/GroundTesselation.frag",GL_FRAGMENT_SHADER,true);
   terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.frag",GL_FRAGMENT_SHADER,true);
   Texture * groundTexture = new TextureFile("terrain/mud.jpg","diffuse");
   Texture * noise = new TextureFile("terrain-noise-blur.jpg","noise");
   terrainMat->addTexture(groundTexture);
   terrainMat->addTexture(noise);
   terrainMat->done(attributes);

   terrainMat->getShaderProgram()->setUniform("TessLevelInner",1.0f);
   terrainMat->getShaderProgram()->setUniform("TessLevelOuter",1.0f);
   terrainMat->getShaderProgram()->setUniform("LightPosition", QVector3D(0.25, 0.25, 1));
   setAtmoUniforms(terrainMat->getShaderProgram());
   Mesh * groundMesh = MeshLoader::load(attributes, "earth.obj");
//    Mesh * mesh = Geometry::gluSphere(10.0f, 100, 50);
   groundMesh->setDrawType(GL_PATCHES);
   node = new Node("ground", planet->position, planet->getSize()*11.5, groundMesh, terrainMat);

//   GUI::Instance().addText("tess", "Tess");
//   GUI::Instance().addText("dist", "Dist");
 }

 void TerrainTesselation::updateTesselation(){
   QVector3D camFromPlanet = SceneData::Instance().getCurrentCamera()->position - planet->position;
   float camDistance = camFromPlanet.length();

   while (camDistance < 11.01){
     SceneData::Instance().getCurrentCamera()->position += SceneData::Instance().getCurrentCamera()->position.normalized() *  0.01;
     SceneData::Instance().getCurrentCamera()->update();
   }
   int maxTess = 60;
   float tessStartDistance = 8;
   float scale = maxTess - (camDistance - tessStartDistance);

//   std::stringstream tess;
//   tess << "Tess " << int(scale);
//   GUI::Instance().updateText("tess",tess.str());
//
//   std::stringstream dist;
//   dist << "Dist " << camDistance;
//   GUI::Instance().updateText("dist",dist.str());

   if (scale > 1){
     terrainMat->getShaderProgram()->use();
     terrainMat->getShaderProgram()->setUniform("TessLevelInner",scale);
     terrainMat->getShaderProgram()->setUniform("TessLevelOuter",scale);
   }
 }

 void TerrainTesselation::draw() {
  updateTesselation();
  SceneData::Instance().getCurrentCamera()->setUniforms(node->getMaterial()->getShaderProgram(), planet->position);
  node->setView(SceneData::Instance().getCurrentCamera());
  node->draw();
}
