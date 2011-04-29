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
#include <math.h>
#include <string>
#include <QApplication>
#include <QPainter>
#include "System/TemplateEngine.h"
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Material/ProcTextures.h"
#include "Material/Textures.h"
#include "Material/ShaderProgram.h"

const bool post = true;

class TesselationApp: public Application {
 public:
  Material *textureMaterial, *postMaterial;

  Camera * camera;
  Light * light;
  Node * groundNode;
  QVector3D lightDirection;
  FrameBuffer * fbo;
  Texture * targetTexture;

  explicit TesselationApp() {
  }

  ~TesselationApp() {}

  void scene() {
    Texture * earthMap = new TextureFile("earthmap1k.jpg",
        "color");

    QList<string> attributes;
    attributes.push_back("uv");

    textureMaterial = new Template("Texture",attributes);
    textureMaterial->addTexture(earthMap);

    camera = SceneData::Instance().getCurrentCamera();
    camera->setPosition(QVector3D(0, 0, 25));
    camera->update();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);

    Mesh * innerSphere = Geometry::sphere(attributes, 10.0f, 100, 50);

    groundNode = new Node("ground", { 0, 0, 0 }, 1, innerSphere,
        textureMaterial);

    unsigned width = MediaLayer::Instance().width;
    unsigned height = MediaLayer::Instance().height;

    fbo = new FrameBuffer(width, height);
    targetTexture = new ColorTexture(width, height,
        "targetTexture");
    fbo->attachTexture(GL_COLOR_ATTACHMENT0, targetTexture);

    postMaterial = new Template("Post/HDR", attributes);
    postMaterial->addTexture(targetTexture);
    postMaterial->shaderProgram->setUniform("exposure", 2.0f);
    fbo->checkAndFinish();

  }
  void renderFrame() {
    if(post){
      fbo->bind();
      fbo->updateRenderView();
    }
    RenderEngine::Instance().clear();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    groundNode->setView(camera);
    groundNode->draw();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    GUI::Instance().draw();
    glError;
    if(post){
      fbo->unBind();
      RenderEngine::Instance().clear();
      postMaterial->activateTextures();
      postMaterial->getShaderProgram()->use();
      fbo->draw(postMaterial);
      glError;
    }
  }
};


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  TesselationApp().run();
}

