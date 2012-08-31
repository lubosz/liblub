/*
 * Terrain.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "TerrainTesselation.h"
#include "Texture/Textures.h"
#include "System/TemplateEngine.h"
#include "Procedural/Geometry.h"
#include "Load/MeshLoader.h"
#include "Atmosphere.h"
#include "Scene/Scene.h"
#include "Planet.h"

TerrainTesselation::TerrainTesselation(Planet * planet) {
  this->planet = planet;
    shader = new ShaderProgram("Tesselation");
}

TerrainTesselation::~TerrainTesselation() {
  // TODO Auto-generated destructor stub
}

void TerrainTesselation::init(){
   QList<string> attributes;
    attributes.push_back("normal");
    attributes.push_back("uv");


//    terrainMat->attachShader("Atmo/GroundTesselation.vert",GL_VERTEX_SHADER);
   shader->attachShader("Tesselation/Tesselation.vert",GL_VERTEX_SHADER);
   shader->attachShader("Tesselation/Tesselation.eval",GL_TESS_EVALUATION_SHADER);
   shader->attachShader("Tesselation/Tesselation.cont",GL_TESS_CONTROL_SHADER);
   shader->attachShader("Tesselation/Tesselation.geom",GL_GEOMETRY_SHADER);
//    terrainMat->attachShader("Atmo/GroundTesselation.frag",GL_FRAGMENT_SHADER);
   shader->attachShader("Tesselation/Tesselation.frag",GL_FRAGMENT_SHADER);
   Texture * groundTexture = new TextureFile("terrain/mud.jpg","diffuse");
   Texture * noise = new TextureFile("terrain-noise-blur.jpg","noise");
   shader->addTexture(groundTexture);
   shader->addTexture(noise);
   shader->init(attributes);

   shader->setUniform("TessLevelInner",1.0f);
   shader->setUniform("TessLevelOuter",1.0f);
   shader->setUniform("LightPosition", QVector3D(0.25, 0.25, 1));
   setAtmoUniforms(shader);
   Mesh * groundMesh = MeshLoader::load(attributes, "earth.obj");
//    Mesh * mesh = Geometry::gluSphere(10.0f, 100, 50);
   groundMesh->setDrawType(GL_PATCHES);
   node = new Node("ground", planet->position, planet->getSize()*11.5, groundMesh, shader);

//   GUI::Instance().addText("tess", "Tess");
//   GUI::Instance().addText("dist", "Dist");
 }

 void TerrainTesselation::updateTesselation(){
   QVector3D camFromPlanet = Scene::Instance().getCurrentCamera()->position - planet->position;
   float camDistance = camFromPlanet.length();

   while (camDistance < 11.01){
     Scene::Instance().getCurrentCamera()->position += Scene::Instance().getCurrentCamera()->position.normalized() *  0.01;
     Scene::Instance().getCurrentCamera()->update();
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
     shader->use();
     shader->setUniform("TessLevelInner",scale);
     shader->setUniform("TessLevelOuter",scale);
   }
 }

 void TerrainTesselation::draw() {
  updateTesselation();
  Scene::Instance().getCurrentCamera()->setUniforms(node->getShader(), planet->position);
  node->setView(Scene::Instance().getCurrentCamera());
  node->draw();
}
