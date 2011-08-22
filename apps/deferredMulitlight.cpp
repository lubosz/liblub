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

class DefferedLightApp: public Application {
 public:
  Material *HDR;
  bool useHDR;
//  Camera* camera;
//  Light * light;
  FrameBuffer *fbo;
//  RenderSequence * shadowSequence;

  DefferedLightApp() {
    sceneLoader = new SceneLoader("multilight.xml");
    useHDR = true;
    fontOverlay = true;
  }

  ~DefferedLightApp() {}

  void initDumpMaterial() {

  }

  void initPostProcessing(){
    if(useHDR){
      unsigned width = SceneData::Instance().width;
      unsigned height = SceneData::Instance().height;

      fbo = new FrameBuffer(width, height);
      Texture * positionTexture = new ColorTexture(width, height, "positionTexture");
      Texture * normalTexture = new ColorTexture(width, height, "normalTexture");
      Texture * diffuseTexture = new ColorTexture(width, height, "diffuseTexture");
      Texture * tangentTexture = new ColorTexture(width, height, "tangentTexture");
      Texture * normalTextureTexture = new ColorTexture(width, height, "normalTextureTexture");
      Texture * envTexture = new ColorTexture(width, height, "envTexture");

      fbo->attachTexture(GL_COLOR_ATTACHMENT0, positionTexture);
      fbo->attachTexture(GL_COLOR_ATTACHMENT1, normalTexture);
      fbo->attachTexture(GL_COLOR_ATTACHMENT2, diffuseTexture);
      fbo->attachTexture(GL_COLOR_ATTACHMENT3, tangentTexture);
      fbo->attachTexture(GL_COLOR_ATTACHMENT4, normalTextureTexture);
      fbo->attachTexture(GL_COLOR_ATTACHMENT5, envTexture);

      QList<string> attributes;
      attributes.push_back("uv");

      HDR = new Template("Post/DeferredMultiLight",attributes);
      HDR->addTexture(positionTexture);
      HDR->addTexture(normalTexture);
      HDR->addTexture(diffuseTexture);
      HDR->addTexture(tangentTexture);
      HDR->addTexture(normalTextureTexture);
      HDR->addTexture(envTexture);
//      HDR->shaderProgram->setUniform("exposure", 2.0f);
      fbo->checkAndFinish();
    }
  }

  void startPass(){

//    useHDR = !RenderEngine::Instance().wire;

    if(useHDR) {
      fbo->bindMulti();
      fbo->updateRenderView();
    }
  }

  void endPass(){
    if(useHDR){
      fbo->unBind();
      RenderEngine::Instance().clear();
      HDR->activateTextures();
      HDR->getShaderProgram()->use();
      HDR->getShaderProgram()->setUniform("camPosition", SceneData::Instance().getCurrentCamera()->position.toVector4D());
      fbo->draw(HDR);
    }
  }

  void scene() {
    ShaderProgram * fistPass = new ShaderProgram();
    fistPass->attachVertFrag("Post/MultiTarget", true);
    GLuint multiTargetProgram = fistPass->getHandle();
    glBindFragDataLocation(multiTargetProgram, 0, "fragColor");
    glBindFragDataLocation(multiTargetProgram, 1, "normalColor");
    glBindFragDataLocation(multiTargetProgram, 2, "diffuseColor");
    glBindFragDataLocation(multiTargetProgram, 3, "tangentColor");
    glBindFragDataLocation(multiTargetProgram, 4, "normalTextureColor");
    glBindFragDataLocation(multiTargetProgram, 5, "envColor");
    string programname = "multilight";

    QList<string> attributes;
    attributes.push_back("uv");
    attributes.push_back("normal");
    attributes.push_back("tangent");

    fistPass->init(attributes);
    fistPass->name = programname;
    SceneData::Instance().addProgram(programname,fistPass);



    sceneLoader->load();
//    Material * multilightMat = SceneData::Instance().getMaterial("white");

    //    multilightMat->addTexture(shadowSequence->renderPasses[0]->targetTexture);

    initPostProcessing();
    SceneData::Instance().initLightBuffer(HDR->getShaderProgram(),
        "LightSourceBuffer");
  }

  void renderFrame(){
    startPass();
    RenderEngine::Instance().clear();
    SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
    endPass();
    GUI::Instance().draw();
    glError;
  }
};

int main(int argc, char *argv[]) {
  DefferedLightApp().run();
  return 0;
}

