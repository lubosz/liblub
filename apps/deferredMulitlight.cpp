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
#include "common/OpenGL.h"
#include "System/Application.h"
#include "Material/Textures.h"
#include "Mesh/Geometry.h"
#include "Renderer/RenderEngine.h"
#include "Renderer/FrameBuffer.h"
#include "Scene/SceneLoader.h"
#include "Material/Materials.h"
#include "Scene/SceneData.h"
#include "Renderer/RenderPasses.h"

class SourcePass : public OutPass{
public:
  Texture * positionTarget, *normalTarget, *diffuseTarget, *tangentTarget,
      *normalMapTarget, *envTarget, *depthTarget;

  explicit SourcePass(QSize res) : OutPass(res) {
    initTargets();
    initFBO();
    initShader();
    initSamplers();
  }

  void initShader() {
    QList<string> attributes2 = QList<string> () << "uv" << "normal"
        << "tangent";
    material = new Template("Post/MultiTarget", attributes2);
    GLuint program = material->getShaderProgram()->getHandle();
    glBindFragDataLocation(program, 0, "positionTarget");
    glBindFragDataLocation(program, 1, "normalTarget");
    glBindFragDataLocation(program, 2, "diffuseTarget");
    glBindFragDataLocation(program, 3, "tangentTarget");
    glBindFragDataLocation(program, 4, "normalMapTarget");
    glBindFragDataLocation(program, 5, "envTarget");
    glBindFragDataLocation(program, 6, "depthTarget");
  }

  void initFBO() {
    fbo = new FrameBuffer(res);
    fbo->attachTexture(GL_COLOR_ATTACHMENT0, positionTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT1, normalTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT2, diffuseTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT3, tangentTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT4, normalMapTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT5, envTarget);
    fbo->setDrawBuffers(6);
    fbo->attachTexture(GL_DEPTH_ATTACHMENT, depthTarget);
    fbo->check();
  }

  void initSamplers() {
    Texture * diffuseTexture = SceneData::Instance().getTexture(
        "masonry-wall-texture");
    diffuseTexture->name = "diffuseTexture";
    Texture * normalTexture = SceneData::Instance().getTexture(
        "masonry-wall-normal-map");
    normalTexture->name = "normalTexture";
    Texture * envMap = SceneData::Instance().getTexture("sky");
    envMap->name = "envMap";

    material->addTexture(diffuseTexture);
    material->addTexture(normalTexture);
    material->addTexture(envMap);

    material->activateTextures();
    material->samplerUniforms();
  }

  void initTargets() {
    positionTarget = new ColorTexture(res, "positionTarget");
    normalTarget = new ColorTexture(res, "normalTarget");
    diffuseTarget = new ColorTexture(res, "diffuseTarget");
    tangentTarget = new ColorTexture(res, "tangentTarget");
    normalMapTarget = new ColorTexture(res, "normalMapTarget");
    envTarget = new ColorTexture(res, "envTarget");
    depthTarget = new DepthTexture(res, "depthTarget");
  }
};

class AOPass: public InOutPass {
public:

  Texture * aoTexture;
  explicit AOPass(QSize res, Texture * normalTarget, Texture *depthTarget) :
    InOutPass(res) {
    aoTexture = new ColorTexture(res, "ao");
    Texture * noise = new TextureFile("noise.png", "noise");

    fbo = new FrameBuffer(res);
    fbo->attachTexture(GL_COLOR_ATTACHMENT0, aoTexture);
    fbo->check();

    QList<string> attributes;
    attributes.push_back("uv");
    material = new Simple("AO/ssao", attributes);
    material->addTexture(normalTarget);
    material->addTexture(depthTarget);
    material->addTexture(noise);
  }
};

class BlurHPass : public InOutPass{
public:
  Texture * blurHTexture;
  explicit BlurHPass(QSize res, Texture * aoTexture) : InOutPass(res) {
    blurHTexture = new ColorTexture(res, "blurH");

    fbo = new FrameBuffer(res);
    fbo->attachTexture(GL_COLOR_ATTACHMENT0, blurHTexture);
    fbo->check();

    QList<string> attributes;
    attributes.push_back("uv");
    material = new Simple("AO/blur_horizontal", attributes);
    material->addTexture(aoTexture);
  }
};

class BlurVPass : public InOutPass{
public:
  Texture * finalAOTarget;
  explicit BlurVPass(QSize res, Texture * blurHTexture) : InOutPass(res) {
    finalAOTarget = new ColorTexture(res, "finalAOTarget");

    fbo = new FrameBuffer(res);
    fbo->attachTexture(GL_COLOR_ATTACHMENT0, finalAOTarget);
    fbo->check();

    QList<string> attributes;
    attributes.push_back("uv");
    material = new Simple("AO/blur_vertical", attributes);
    material->addTexture(blurHTexture);
  }
};

class ShadingPass : public SinkPass {
public:

  explicit ShadingPass(QSize res, Texture * positionTarget, Texture *normalTarget,
      Texture *diffuseTarget, Texture *tangentTarget, Texture *normalMapTarget,
      Texture *envTarget, Texture *finalAOTarget) : SinkPass(res) {

    // Deffered Light Shader
    QList<string> attributes = QList<string> () << "uv";
    material = new Template("Post/DeferredMultiLight", attributes);
    material->addTexture(positionTarget);
    material->addTexture(normalTarget);
    material->addTexture(diffuseTarget);
    material->addTexture(tangentTarget);
    material->addTexture(normalMapTarget);
    material->addTexture(envTarget);
    material->addTexture(finalAOTarget);

    // Init Light Uniform Buffer
    SceneData::Instance().initLightBuffer(material->getShaderProgram(),
        "LightSourceBuffer");

    debugTarget(QRectF(0.5, -1, 0.5, 0.5), normalTarget);
    debugTarget(QRectF(0.5, -0.5, 0.5, 0.5), diffuseTarget);
    debugTarget(QRectF(0.5, 0, 0.5, 0.5), finalAOTarget);
  }
};

class DeferredLightApp: public Application {
public:
  SceneLoader *sceneLoader;
  vector<DrawPass*> drawPasses;

  DeferredLightApp(int argc, char *argv[]) :
    Application(argc, argv) {
    sceneLoader = new SceneLoader("multilight.xml");
    fontOverlay = false;
  }

  ~DeferredLightApp() {
  }

  void scene() {
    sceneLoader->load();
    QSize res = SceneData::Instance().getResolution();

    SourcePass * gatherPass;
    gatherPass = new SourcePass(res);
    drawPasses.push_back(gatherPass);

    AOPass * aoPass;
    aoPass = new AOPass(res, gatherPass->normalTarget, gatherPass->depthTarget);
    drawPasses.push_back(aoPass);

    BlurHPass * blurHPass;
    blurHPass = new BlurHPass(res, aoPass->aoTexture);
    drawPasses.push_back(blurHPass);

    BlurVPass * blurVPass;
    blurVPass = new BlurVPass(res, blurHPass->blurHTexture);
    drawPasses.push_back(blurVPass);

    ShadingPass * shadingPass;
    shadingPass = new ShadingPass(res, gatherPass->positionTarget,
        gatherPass->normalTarget, gatherPass->diffuseTarget,
        gatherPass->tangentTarget, gatherPass->normalMapTarget,
        gatherPass->envTarget, blurVPass->finalAOTarget);
    drawPasses.push_back(shadingPass);
  }

  void renderFrame() {
    foreach(DrawPass * pass, drawPasses)
        pass->draw();
  }

  void initWidgets(QHBoxLayout * mainLayout) {
  }
};

int main(int argc, char *argv[]) {
  DeferredLightApp(argc, argv).run();
  return 0;
}

