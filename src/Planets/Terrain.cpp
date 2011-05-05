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

Terrain::Terrain() {
  // TODO Auto-generated constructor stub

}

Terrain::~Terrain() {
  // TODO Auto-generated destructor stub
}

void Terrain::init(float innerRadius){
   QList<string> attributes;
    attributes.push_back("normal");
    attributes.push_back("uv");

  Texture * earthMap = new TextureFile("earthmap1k.jpg",
      "planet");
  groundFromAtmosphere = new Template("Atmo/Ground",attributes);
  groundFromAtmosphere->addTexture(earthMap);
  TemplateEngine::Instance().c.insert("fromSpace", true);
  groundFromSpace = new Template("Atmo/Ground",attributes);
  groundFromSpace->addTexture(earthMap);

  Mesh * innerSphere = Geometry::sphere(attributes, innerRadius, 100, 50);
  groundNode = new Node("ground", { 0, 0, 0 }, 1, innerSphere, groundFromAtmosphere);
  groundNode->setRotation(QVector3D(-90, 0, 0));
  setAtmoUniforms(groundFromAtmosphere->getShaderProgram());
  setAtmoUniforms(groundFromSpace->getShaderProgram());


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
   terrainNode = new Node("ground", { 0, 0, 0 }, 12.1, groundMesh, terrainMat);

   GUI::Instance().addText("tess", "Tess");
   GUI::Instance().addText("dist", "Dist");
 }

 void Terrain::updateTesselation(){
   while (camera->position.length() < 11.01){
     camera->position += camera->position.normalized() *  0.01;
     camera->update();
   }
   int maxTess = 60;
   float tessStartDistance = 8;
   float scale = maxTess - (camera->position.length() - tessStartDistance);

   std::stringstream tess;
   tess << "Tess " << int(scale);
   GUI::Instance().updateText("tess",tess.str());

   std::stringstream dist;
   dist << "Dist " << camera->position.length();
   GUI::Instance().updateText("dist",dist.str());

   if (scale > 1){
     terrainMat->getShaderProgram()->use();
     terrainMat->getShaderProgram()->setUniform("TessLevelInner",scale);
     terrainMat->getShaderProgram()->setUniform("TessLevelOuter",scale);
   }
 }

 void Terrain::draw() {
  updateTesselation();
  if (camera->position.length() >= outerRadius)
    groundNode->setMaterial(groundFromSpace);
  else
    groundNode->setMaterial(groundFromAtmosphere);

  setCameraUniforms(groundNode->getMaterial()->getShaderProgram());
  groundNode->setView(camera);

  groundNode->draw();

  setCameraUniforms(terrainNode->getMaterial()->getShaderProgram());
  terrainNode->setView(camera);
  terrainNode->draw();
}
