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
#include "Mesh/MeshFactory.h"

class TesselationApp: public Application {
 public:
  Node * groundNode;
  Camera * camera;
  Material *material;

  explicit TesselationApp() {
  }

  ~TesselationApp() {}

  void scene() {
    Light * light;

    QList<string> attributes;

    material = new EmptyMat();

    material->init();
    material->getShaderProgram()->attachShader("Tesselation/Tesselation.vert",GL_VERTEX_SHADER,true);
    material->getShaderProgram()->attachShader("Tesselation/Tesselation.eval",GL_TESS_EVALUATION_SHADER,true);
    material->getShaderProgram()->attachShader("Tesselation/Tesselation.cont",GL_TESS_CONTROL_SHADER,true);
    material->getShaderProgram()->attachShader("Tesselation/Tesselation.geom",GL_GEOMETRY_SHADER,true);
    material->getShaderProgram()->attachShader("Tesselation/Tesselation.frag",GL_FRAGMENT_SHADER,true);
    material->done(attributes);

    material->getShaderProgram()->setUniform("TessLevelInner",1.0f);
    material->getShaderProgram()->setUniform("TessLevelOuter",1.0f);
    material->getShaderProgram()->setUniform("LightPosition", QVector3D(0.25, 0.25, 1));
    material->getShaderProgram()->setUniform("DiffuseMaterial",QVector3D(0, 0.75, 0.75));
    material->getShaderProgram()->setUniform("AmbientMaterial",QVector3D(0.04f, 0.04f, 0.04f));

    camera = SceneData::Instance().getCurrentCamera();
    camera->setPosition(QVector3D(0, 0, 25));
    camera->update();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);

    Mesh * mesh = MeshFactory::load("earth.obj");
//    Mesh * mesh = Geometry::gluSphere(10.0f, 100, 50);
//    Mesh * mesh = Geometry::makeIcosahedron();
    mesh->setDrawType(GL_PATCHES);

    groundNode = new Node("ground", { 0, 0, 0 }, 10, mesh, material);
    groundNode->setRotation(QVector3D(90,0,0));
    glError;

    GUI::Instance().addText("tess", "Tess");
    GUI::Instance().addText("dist", "Dist");

    Texture * groundTexture = TextureFactory::Instance().load("terrain/mud.jpg","diffuse");
    Texture * noise = TextureFactory::Instance().load("terrain-noise-blur.jpg","noise");
    material->addTexture(groundTexture);
    material->addTexture(noise);
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
      material->getShaderProgram()->use();
      material->getShaderProgram()->setUniform("TessLevelInner",scale);
      material->getShaderProgram()->setUniform("TessLevelOuter",scale);
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
  QApplication app(argc, argv);
  TesselationApp().run();
}

