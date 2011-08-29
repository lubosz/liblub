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

class GoodRenderPass {
public:
  QSize res;
  explicit GoodRenderPass(QSize res) {
    this->res = res;
  }
  virtual void draw() = 0;

  static void drawOnPlane(Material * material, Mesh *plane) {
    material->getShaderProgram()->use();
    material->getShaderProgram()->setUniform("MVPMatrix", QMatrix4x4());
    material->activate();
    plane->draw();
  }
};

class GatherPass : public GoodRenderPass{
public:
  Material *gatherMat;
  FrameBuffer * fbo;
  Texture * positionTarget, *normalTarget, *diffuseTarget, *tangentTarget,
      *normalMapTarget, *envTarget, *depthTarget;

  explicit GatherPass(QSize res) : GoodRenderPass(res) {}
  void init() {
    // Pass 1 Gather FBO
    fbo = new FrameBuffer(res);
    positionTarget = new ColorTexture(res, "positionTarget");
    normalTarget = new ColorTexture(res, "normalTarget");
    diffuseTarget = new ColorTexture(res, "diffuseTarget");
    tangentTarget = new ColorTexture(res, "tangentTarget");
    normalMapTarget = new ColorTexture(res, "normalMapTarget");
    envTarget = new ColorTexture(res, "envTarget");
    depthTarget = new DepthTexture(res, "depthTarget");

    fbo->attachTexture(GL_COLOR_ATTACHMENT0, positionTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT1, normalTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT2, diffuseTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT3, tangentTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT4, normalMapTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT5, envTarget);
    fbo->setDrawBuffers(6);
    fbo->attachTexture(GL_DEPTH_ATTACHMENT, depthTarget);
    fbo->check();

    // Pass 1 Gather Shader
    QList<string> attributes2 = QList<string> () << "uv" << "normal"
        << "tangent";
    gatherMat = new Template("Post/MultiTarget", attributes2);
    GLuint program = gatherMat->getShaderProgram()->getHandle();
    glBindFragDataLocation(program, 0, "positionTarget");
    glBindFragDataLocation(program, 1, "normalTarget");
    glBindFragDataLocation(program, 2, "diffuseTarget");
    glBindFragDataLocation(program, 3, "tangentTarget");
    glBindFragDataLocation(program, 4, "normalMapTarget");
    glBindFragDataLocation(program, 5, "envTarget");
    glBindFragDataLocation(program, 6, "depthTarget");

    // Pass 1 Gather Textures
    Texture * diffuseTexture = SceneData::Instance().getTexture(
        "masonry-wall-texture");
    diffuseTexture->name = "diffuseTexture";
    Texture * normalTexture = SceneData::Instance().getTexture(
        "masonry-wall-normal-map");
    normalTexture->name = "normalTexture";
    Texture * envMap = SceneData::Instance().getTexture("sky");
    envMap->name = "envMap";

    gatherMat->addTexture(diffuseTexture);
    gatherMat->addTexture(normalTexture);
    gatherMat->addTexture(envMap);

    gatherMat->activateTextures();
    gatherMat->samplerUniforms();
  }
  void draw() {
    fbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    SceneGraph::Instance().drawNodes(gatherMat);
    fbo->unBind();
  }
};

class AOPass : public GoodRenderPass{
public:
  Material *aoMaterial;
  FrameBuffer *aoFbo;
  Mesh * fullPlane;
  Texture * aoTexture;
  explicit AOPass(QSize res) : GoodRenderPass(res) {
    QList<string> attributes;
    attributes.push_back("uv");
    fullPlane = Geometry::plane(attributes, QRectF(-1, -1, 2, 2));
  }

  void init(Texture * normalTarget, Texture *depthTarget) {
    aoTexture = new ColorTexture(res, "ao");
    Texture * noise = new TextureFile("noise.png", "noise");

    aoFbo = new FrameBuffer(res);
    aoFbo->attachTexture(GL_COLOR_ATTACHMENT0, aoTexture);
    aoFbo->check();

    QList<string> attributes;
    attributes.push_back("uv");
    aoMaterial = new Simple("AO/ssao", attributes);
    aoMaterial->addTexture(normalTarget);
    aoMaterial->addTexture(depthTarget);
    aoMaterial->addTexture(noise);
  }

  void draw() {
    aoFbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    drawOnPlane(aoMaterial, fullPlane);
    aoFbo->unBind();
  }
};

class BlurHPass : public GoodRenderPass{
public:
  Material *blur_horizontal;
  FrameBuffer *blurHFbo;
  Mesh * fullPlane;
  Texture * blurHTexture;
  explicit BlurHPass(QSize res) : GoodRenderPass(res) {
    QList<string> attributes;
    attributes.push_back("uv");
    fullPlane = Geometry::plane(attributes, QRectF(-1, -1, 2, 2));
  }

  void init(Texture * aoTexture) {
    blurHTexture = new ColorTexture(res, "blurH");

    blurHFbo = new FrameBuffer(res);
    blurHFbo->attachTexture(GL_COLOR_ATTACHMENT0, blurHTexture);
    blurHFbo->check();

    QList<string> attributes;
    attributes.push_back("uv");
    blur_horizontal = new Simple("AO/blur_horizontal", attributes);
    blur_horizontal->addTexture(aoTexture);
  }

  void draw() {
    blurHFbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    drawOnPlane(blur_horizontal, fullPlane);
    blurHFbo->unBind();
  }
};

class BlurVPass : public GoodRenderPass{
public:
  Material *blur_vertical;
  FrameBuffer *blurVFbo;
  Mesh * fullPlane;
  Texture * finalAOTarget;
  explicit BlurVPass(QSize res) : GoodRenderPass(res) {
    QList<string> attributes;
    attributes.push_back("uv");
    fullPlane = Geometry::plane(attributes, QRectF(-1, -1, 2, 2));
  }

  void init(Texture * blurHTexture) {
    finalAOTarget = new ColorTexture(res, "finalAOTarget");

    blurVFbo = new FrameBuffer(res);
    blurVFbo->attachTexture(GL_COLOR_ATTACHMENT0, finalAOTarget);
    blurVFbo->check();

    QList<string> attributes;
    attributes.push_back("uv");
    blur_vertical = new Simple("AO/blur_vertical", attributes);
    blur_vertical->addTexture(blurHTexture);
  }

  void draw() {
    blurVFbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    drawOnPlane(blur_vertical, fullPlane);
    blurVFbo->unBind();
  }
};

class ShadingPass : public GoodRenderPass{
public:
  Material * multiLightMat, *debugMaterial, *debugMaterial2;
  Mesh * fullPlane, *plane1, *plane2, *plane3, *plane4;

  explicit ShadingPass(QSize res) : GoodRenderPass(res) {}

  void initRenderPlanes() {
    QList<string> attributes = QList<string> () << "uv";
    fullPlane = Geometry::plane(attributes, QRectF(-1, -1, 2, 2));

    plane1 = Geometry::plane(attributes, QRectF(-1, -1, 1, 1));
    plane2 = Geometry::plane(attributes, QRectF(0.5, -1, 0.5, 0.5));
    plane3 = Geometry::plane(attributes, QRectF(0.5, -0.5, 0.5, 0.5));
    plane4 = Geometry::plane(attributes, QRectF(0.5, 0, 0.5, 0.5));
  }

  void init(Texture * positionTarget, Texture *normalTarget,
      Texture *diffuseTarget, Texture *tangentTarget, Texture *normalMapTarget,
      Texture *envTarget, Texture *finalAOTarget) {

    initRenderPlanes();
    // Deffered Light Shader
    QList<string> attributes = QList<string> () << "uv";
    multiLightMat = new Template("Post/DeferredMultiLight", attributes);
    multiLightMat->addTexture(positionTarget);
    multiLightMat->addTexture(normalTarget);
    multiLightMat->addTexture(diffuseTarget);
    multiLightMat->addTexture(tangentTarget);
    multiLightMat->addTexture(normalMapTarget);
    multiLightMat->addTexture(envTarget);
    multiLightMat->addTexture(finalAOTarget);

    // Init Light Uniform Buffer
    SceneData::Instance().initLightBuffer(multiLightMat->getShaderProgram(),
        "LightSourceBuffer");
    initDebugMaterials(normalTarget, diffuseTarget);
  }

  void initDebugMaterials(Texture * target1, Texture *target2) {
    QList<string> attributes = QList<string> () << "uv";
    debugMaterial = new Simple("Texture/debugfbo", attributes);
    debugMaterial->addTexture(target1);

    debugMaterial2 = new Simple("Texture/debugfbo2", attributes);
    debugMaterial2->addTexture(target2);
  }

  void draw() {
    RenderEngine::Instance().clear();
    //    drawOnPlane(blur_vertical, plane2);
    drawOnPlane(debugMaterial, plane3);
    drawOnPlane(debugMaterial2, plane4);
    drawOnPlane(multiLightMat, fullPlane);
  }
};

class DeferredLightApp: public Application {
public:

  SceneLoader *sceneLoader;

  GatherPass * gatherPass;
  ShadingPass * shadingPass;
  AOPass * aoPass;
  BlurHPass * blurHPass;
  BlurVPass * blurVPass;

  DeferredLightApp(int argc, char *argv[]) :
    Application(argc, argv) {
    sceneLoader = new SceneLoader("multilight.xml");
    fontOverlay = false;
  }

  ~DeferredLightApp() {
  }

  void scene() {

    Camera * myCam = new Camera();
    SceneData::Instance().setCurrentCamera(myCam);

    sceneLoader->load();
    QSize res = SceneData::Instance().getResolution();
    gatherPass = new GatherPass(res);
    shadingPass = new ShadingPass(res);
    aoPass = new AOPass(res);
    blurHPass = new BlurHPass(res);
    blurVPass = new BlurVPass(res);

    gatherPass->init();

    aoPass->init(gatherPass->normalTarget, gatherPass->depthTarget);

    blurHPass->init(aoPass->aoTexture);
    blurVPass->init(blurHPass->blurHTexture);

    shadingPass->init(gatherPass->positionTarget, gatherPass->normalTarget,
        gatherPass->diffuseTarget, gatherPass->tangentTarget,
        gatherPass->normalMapTarget, gatherPass->envTarget,
        blurVPass->finalAOTarget);
  }

  void renderFrame() {
    gatherPass->draw();
    aoPass->draw();
    blurHPass->draw();
    blurVPass->draw();
    shadingPass->draw();
  }
  void initWidgets(QHBoxLayout * mainLayout) {
  }
};

int main(int argc, char *argv[]) {
  DeferredLightApp(argc, argv).run();
  return 0;
}

