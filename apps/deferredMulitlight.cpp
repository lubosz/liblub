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

class DefferedLightApp: public Application {
 public:
  Material * multiLightMat, *gatherMat;
  FrameBuffer * fbo;
  QSize res;

  DefferedLightApp() {
    sceneLoader = new SceneLoader("multilight.xml");
    fontOverlay = false;
  }

  ~DefferedLightApp() {}

  void scene() {
    sceneLoader->load();
    res = SceneData::Instance().getResolution();

    // Pass 1 Gather FBO
    fbo = new FrameBuffer(res);
    Texture * positionTarget = new ColorTexture(res, "positionTarget");
    Texture * normalTarget = new ColorTexture(res, "normalTarget");
    Texture * diffuseTarget = new ColorTexture(res, "diffuseTarget");
    Texture * tangentTarget = new ColorTexture(res, "tangentTarget");
    Texture * normalMapTarget = new ColorTexture(res, "normalMapTarget");
    Texture * envTarget = new ColorTexture(res, "envTarget");

    fbo->attachTexture(GL_COLOR_ATTACHMENT0, positionTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT1, normalTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT2, diffuseTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT3, tangentTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT4, normalMapTarget);
    fbo->attachTexture(GL_COLOR_ATTACHMENT5, envTarget);
    fbo->setDrawBuffers(6);
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

    // Pass 1 Gather Textures
    Texture * diffuseTexture = SceneData::Instance().getTexture("masonry-wall-texture");
    diffuseTexture->name = "diffuseTexture";
    Texture * normalTexture = SceneData::Instance().getTexture("masonry-wall-normal-map");
    normalTexture->name = "normalTexture";
    Texture * envMap = SceneData::Instance().getTexture("sky");
    envMap->name = "envMap";

    gatherMat->addTexture(diffuseTexture);
    gatherMat->addTexture(normalTexture);
    gatherMat->addTexture(envMap);

    gatherMat->activateTextures();
    gatherMat->samplerUniforms();

    // Pass 2 Deffered Light Shader
    QList<string> attributes = QList<string> () << "uv";
    multiLightMat = new Template("Post/DeferredMultiLight",attributes);
    multiLightMat->addTexture(positionTarget);
    multiLightMat->addTexture(normalTarget);
    multiLightMat->addTexture(diffuseTarget);
    multiLightMat->addTexture(tangentTarget);
    multiLightMat->addTexture(normalMapTarget);
    multiLightMat->addTexture(envTarget);

    // Pass 2 Register To SceneData
    string programname = "multilight";
    gatherMat->getShaderProgram()->name = programname;
    SceneData::Instance().addProgram(programname,gatherMat->getShaderProgram());

    // Pass 2 Init Light Uniform Buffer
    SceneData::Instance().initLightBuffer(multiLightMat->getShaderProgram(), "LightSourceBuffer");
  }

  void renderFrame(){
    fbo->bind();
    fbo->updateRenderView();
    RenderEngine::Instance().clear();
    SceneGraph::Instance().drawCasters(gatherMat);
    fbo->unBind();
    RenderEngine::Instance().clear();

    fbo->draw(multiLightMat);
  }
};

int main(int argc, char *argv[]) {
  DefferedLightApp().run();
  return 0;
}

