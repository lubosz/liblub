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

#include "System/Application.h"
#include "Material/Textures.h"
#include "Mesh/Geometry.h"

class DefferedLightApp: public Application {
 public:
  Material * multiLightMat, *gatherMat, *aoMaterial, *blur_horizontal,
      *blur_vertical, *debugMaterial, *debugMaterial2;
  FrameBuffer * fbo, *aoFbo, *blurHFbo, *blurVFbo;
  QSize res;

  Texture * positionTarget, *normalTarget, *diffuseTarget, *tangentTarget,
      *normalMapTarget, *envTarget, *depthTarget,* finalAOTarget;

  Mesh * fullPlane, *plane1, *plane2, *plane3, *plane4;

  SceneLoader *sceneLoader;

  DefferedLightApp(int argc, char *argv[]) : Application(argc,argv) {
    sceneLoader = new SceneLoader("multilight.xml");
    fontOverlay = false;
  }

  ~DefferedLightApp() {}

  void initAOPass() {
    //PASS 2 AO
    Texture * aoTexture = new ColorTexture(res, "ao");
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

    //PASS 3 BlurH
    Texture * blurHTexture = new ColorTexture(res, "blurH");

    blurHFbo = new FrameBuffer(res);
    blurHFbo->attachTexture(GL_COLOR_ATTACHMENT0, blurHTexture);
    blurHFbo->check();

    blur_horizontal = new Simple("AO/blur_horizontal", attributes);
    blur_horizontal->addTexture(aoTexture);

    //PASS 4 BlurV
    finalAOTarget = new ColorTexture(res, "finalAOTarget");

    blurVFbo = new FrameBuffer(res);
    blurVFbo->attachTexture(GL_COLOR_ATTACHMENT0, finalAOTarget);
    blurVFbo->check();


    blur_vertical = new Simple("AO/blur_vertical", attributes);
    blur_vertical->addTexture(blurHTexture);
  }

  void initGatherPass() {
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
    QList<string> attributes2 =
        QList<string> () << "uv" << "normal" << "tangent";
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

  void initShadingPass() {
    // Pass 2 Deffered Light Shader
    QList<string> attributes = QList<string> () << "uv";
    multiLightMat = new Template("Post/DeferredMultiLight",attributes);
    multiLightMat->addTexture(positionTarget);
    multiLightMat->addTexture(normalTarget);
    multiLightMat->addTexture(diffuseTarget);
    multiLightMat->addTexture(tangentTarget);
    multiLightMat->addTexture(normalMapTarget);
    multiLightMat->addTexture(envTarget);
    multiLightMat->addTexture(finalAOTarget);

    // Pass 2 Register To SceneData
    string programname = "multilight";
    gatherMat->getShaderProgram()->name = programname;
    SceneData::Instance().addProgram(programname,gatherMat->getShaderProgram());

    // Pass 2 Init Light Uniform Buffer
    SceneData::Instance().initLightBuffer(multiLightMat->getShaderProgram(), "LightSourceBuffer");
  }

  void drawOnPlane(Material * material, Mesh *plane) {
    material->getShaderProgram()->use();
    material->getShaderProgram()->setUniform("MVPMatrix",QMatrix4x4());
    material->activate();
    plane->draw();
  }

  void scene() {
    QList<string> attributes = QList<string> () << "uv";
    fullPlane = Geometry::plane(attributes, QRectF(-1,-1,2,2));

    plane1 = Geometry::plane(attributes, QRectF(-1,-1,1,1));
    plane2 = Geometry::plane(attributes, QRectF(0.5,-1,0.5,0.5));
    plane3 = Geometry::plane(attributes, QRectF(0.5,-0.5,0.5,0.5));
    plane4 = Geometry::plane(attributes, QRectF(0.5,0,0.5,0.5));

    sceneLoader->load();
    res = SceneData::Instance().getResolution();
    initGatherPass();
    initAOPass();
    initShadingPass();

    debugMaterial = new Simple("Texture/debugfbo", attributes);
    debugMaterial->addTexture(normalTarget);

    debugMaterial2 = new Simple("Texture/debugfbo2", attributes);
    debugMaterial2->addTexture(diffuseTarget);
  }

  void renderFrame(){
    fbo->bind();
    fbo->updateRenderView();
    RenderEngine::Instance().clear();
    SceneGraph::Instance().drawCasters(gatherMat);
    fbo->unBind();

    aoFbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    drawOnPlane(aoMaterial, fullPlane);
    aoFbo->unBind();

    blurHFbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    drawOnPlane(blur_horizontal, fullPlane);
    blurHFbo->unBind();

    blurVFbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    drawOnPlane(blur_vertical, fullPlane);
    blurVFbo->unBind();

    RenderEngine::Instance().clear();
    drawOnPlane(blur_vertical, plane2);
    drawOnPlane(debugMaterial, plane3);
    drawOnPlane(debugMaterial2, plane4);
    drawOnPlane(multiLightMat, fullPlane);
  }
};

int main(int argc, char *argv[]) {
  DefferedLightApp(argc,argv).run();
  return 0;
}

