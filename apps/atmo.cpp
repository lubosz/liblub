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
#include <string>
#include <QApplication>
#include <QPainter>
#include "System/TemplateEngine.h"
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Material/ProcTextures.h"
#include "Material/Textures.h"
#include "Material/ShaderProgram.h"
#include "Planets/Planet.h"

class AtmosphereApp: public Application {
 public:
  Material *HDR;
  bool useHDR;
  Camera* camera;
  Light * light;

  FrameBuffer *fbo;
  Planet * planet;

  AtmosphereApp() {
    useHDR = true;
    planet = new Planet();
  }

  ~AtmosphereApp() {}


  void setCameraUniforms(ShaderProgram * program){
    program->use();
    program->setUniform("cameraPosition", camera->position);
    program->setUniform("cameraHeight", (float)camera->position.length());
    program->setUniform("cameraHeight2", float(camera->position.length()*camera->position.length()));
  }

  void initCamAndLight(){
    camera = SceneData::Instance().getCurrentCamera();
    camera->setPosition(QVector3D(0, 0, 25));
    camera->update();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);
  }

  void initPostProcessing(){
    if(useHDR){
      unsigned width = MediaLayer::Instance().width;
      unsigned height = MediaLayer::Instance().height;

      fbo = new FrameBuffer(width, height);
      Texture * targetTexture = new ColorTexture(width, height, "targetTexture");
      fbo->attachTexture(GL_COLOR_ATTACHMENT0, targetTexture);

      QList<string> attributes;
      attributes.push_back("uv");

      HDR = new Template("Post/HDR",attributes);
      HDR->addTexture(targetTexture);
      HDR->shaderProgram->setUniform("exposure", 2.0f);
      fbo->checkAndFinish();
    }
  }

  void startPass(){
    if(useHDR) {
      fbo->bind();
      fbo->updateRenderView();
    }
  }

  void endPass(){
    if(useHDR){
      fbo->unBind();
      RenderEngine::Instance().clear();
      HDR->activateTextures();
      HDR->getShaderProgram()->use();
      fbo->draw(HDR);
    }
  }

  void scene() {
    planet-> init();
    initCamAndLight();
    initPostProcessing();
  }

  void renderFrame(){
    startPass();
    RenderEngine::Instance().clear();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    planet->draw();
    endPass();
    GUI::Instance().draw();
    glError;
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  AtmosphereApp().run();
}

