/*
 * Terrain.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "Terrain.h"
#include "Material/Textures.h"
#include "Material/Materials.h"
#include "System/TemplateEngine.h"
#include "System/GUI.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Atmosphere.h"
#include "Scene/SceneData.h"

Terrain::Terrain(float innerRadius, float outerRadius) {
  this->innerRadius = innerRadius;
  this->outerRadius = outerRadius;
}

Terrain::~Terrain() {
  // TODO Auto-generated destructor stub
}

void Terrain::init(const QVector3D& position, float size){
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
  Atmosphere::setAtmoUniforms(groundFromAtmosphere->getShaderProgram(),innerRadius, outerRadius);
  Atmosphere::setAtmoUniforms(groundFromSpace->getShaderProgram(),innerRadius, outerRadius);


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
   Atmosphere::setAtmoUniforms(terrainMat->getShaderProgram(),innerRadius, outerRadius);
   Mesh * groundMesh = MeshLoader::load(attributes, "earth.obj");
//    Mesh * mesh = Geometry::gluSphere(10.0f, 100, 50);
   groundMesh->setDrawType(GL_PATCHES);
   terrainNode = new Node("ground", position, size*12.1, groundMesh, terrainMat);

   GUI::Instance().addText("tess", "Tess");
   GUI::Instance().addText("dist", "Dist");
 }

 void Terrain::updateTesselation(){
   while (SceneData::Instance().getCurrentCamera()->position.length() < 11.01){
     SceneData::Instance().getCurrentCamera()->position += SceneData::Instance().getCurrentCamera()->position.normalized() *  0.01;
     SceneData::Instance().getCurrentCamera()->update();
   }
   int maxTess = 60;
   float tessStartDistance = 8;
   float scale = maxTess - (SceneData::Instance().getCurrentCamera()->position.length() - tessStartDistance);

   std::stringstream tess;
   tess << "Tess " << int(scale);
   GUI::Instance().updateText("tess",tess.str());

   std::stringstream dist;
   dist << "Dist " << SceneData::Instance().getCurrentCamera()->position.length();
   GUI::Instance().updateText("dist",dist.str());

   if (scale > 1){
     terrainMat->getShaderProgram()->use();
     terrainMat->getShaderProgram()->setUniform("TessLevelInner",scale);
     terrainMat->getShaderProgram()->setUniform("TessLevelOuter",scale);
   }
 }

 void Terrain::draw() {
  updateTesselation();
  if (SceneData::Instance().getCurrentCamera()->position.length() >= outerRadius)
    groundNode->setMaterial(groundFromSpace);
  else
    groundNode->setMaterial(groundFromAtmosphere);

  SceneData::Instance().getCurrentCamera()->setUniforms(groundNode->getMaterial()->getShaderProgram(), position);
  groundNode->setView(SceneData::Instance().getCurrentCamera());

  groundNode->draw();

  SceneData::Instance().getCurrentCamera()->setUniforms(terrainNode->getMaterial()->getShaderProgram(), position);
  terrainNode->setView(SceneData::Instance().getCurrentCamera());
  terrainNode->draw();
}
