/*
    Copyright © 2010 Lubosz Sarnecki

    This file is part of liblub.

    liblub is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    liblub is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with liblub.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QApplication>
#include "System/Application.h"
#include "System/Logger.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Material/Textures.h"

class TesselationApp: public Application {
 public:
  Node * groundNode;
  Camera * camera;
  Material *terrainMat;

  explicit TesselationApp() {
  }

  ~TesselationApp() {}

  void scene() {
    Light * light;

    QList<string> attributes;

    terrainMat = new EmptyMat();

    terrainMat->init();
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.vert",GL_VERTEX_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.eval",GL_TESS_EVALUATION_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.cont",GL_TESS_CONTROL_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.geom",GL_GEOMETRY_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.frag",GL_FRAGMENT_SHADER,true);
    terrainMat->done(attributes);

    terrainMat->getShaderProgram()->setUniform("TessLevelInner",1.0f);
    terrainMat->getShaderProgram()->setUniform("TessLevelOuter",1.0f);
    terrainMat->getShaderProgram()->setUniform("LightPosition", QVector3D(0.25, 0.25, 1));

    camera = SceneData::Instance().getCurrentCamera();
    camera->setPosition(QVector3D(0, 0, 25));
    camera->update();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);

    Mesh * mesh = MeshLoader::load(attributes, "earth.obj");
//    Mesh * mesh = Geometry::gluSphere(10.0f, 100, 50);
//    Mesh * mesh = Geometry::makeIcosahedron();
    mesh->setDrawType(GL_PATCHES);

    groundNode = new Node("ground", { 0, 0, 0 }, 10, mesh, terrainMat);
    groundNode->setRotation(QVector3D(90,0,0));
    glError;

    GUI::Instance().addText("tess", "Tess");
    GUI::Instance().addText("dist", "Dist");

    Texture * groundTexture = new TextureFile("terrain/mud.jpg","diffuse");
    Texture * noise = new TextureFile("terrain-noise-blur.jpg","noise");
    terrainMat->addTexture(groundTexture);
    terrainMat->addTexture(noise);
    RenderEngine::Instance().setWire(true);
  }
  void renderFrame() {
      int maxTess = 30;
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
    RenderEngine::Instance().clear();
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    groundNode->setView(camera);
    glError;
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    groundNode->draw();
    GUI::Instance().draw();
    glError;
  }
};


int main(int argc, char *argv[]) {
  TesselationApp().run();
}

