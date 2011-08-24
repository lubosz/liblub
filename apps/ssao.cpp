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

class SSAOExample: public Application {
 public:

   Material * aoMaterial, * gatherPassMaterial, *blur_horizontal,  *blur_vertical;
   FrameBuffer * fbo, *colorFbo, *aoFbo, *blurHFbo;
   QSize res;

  explicit SSAOExample() {
    fontOverlay = false;
    sceneLoader = new SceneLoader("nice.xml");
  }

  ~SSAOExample() {}

  void scene() {
    sceneLoader->load();

    SceneData::Instance().name = "SSAO";

    res = SceneData::Instance().getResolution();

    //PASS 1 Gather
    Texture * normalTexture = new ColorTexture(res, "normal");
    Texture * depthTexture = new DepthTexture(res, "depth");

    fbo = new FrameBuffer(res);
    fbo->attachTexture(GL_COLOR_ATTACHMENT0, normalTexture);
    fbo->attachTexture(GL_DEPTH_ATTACHMENT, depthTexture);
    fbo->check();

    gatherPassMaterial = new Simple("AO/gather",QList<string>() << "normal");

    GLuint program = gatherPassMaterial->getShaderProgram()->getHandle();
    glBindFragDataLocation(program, 0, "normal");
    glBindFragDataLocation(program, 1, "depth");

    //PASS 2 AO
    Texture * aoTexture = new ColorTexture(res, "ao");
    Texture * noise = new TextureFile("noise.png", "noise");

    aoFbo = new FrameBuffer(res);
    aoFbo->attachTexture(GL_COLOR_ATTACHMENT0, aoTexture);
    aoFbo->check();

    QList<string> attributes;
    attributes.push_back("uv");
    aoMaterial = new Simple("AO/ssao",attributes);
    aoMaterial->addTexture(normalTexture);
    aoMaterial->addTexture(depthTexture);
    aoMaterial->addTexture(noise);

    //PASS 3 BlurH
    Texture * blurHTexture = new ColorTexture(res, "blurH");

    blurHFbo = new FrameBuffer(res);
    blurHFbo->attachTexture(GL_COLOR_ATTACHMENT0, blurHTexture);
    blurHFbo->check();

    blur_horizontal = new Simple("AO/blur_horizontal", attributes);
    blur_horizontal->addTexture(aoTexture);

    //PASS 4 BlurV
    blur_vertical = new Simple("AO/blur_vertical", attributes);
    blur_vertical->addTexture(blurHTexture);

  }
  void renderFrame(){
    fbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    gatherPassMaterial->activate();
    SceneGraph::Instance().drawCasters(gatherPassMaterial);
    fbo->unBind();

    aoFbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    fbo->draw(aoMaterial);
    aoFbo->unBind();

    blurHFbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    fbo->draw(blur_horizontal);
    blurHFbo->unBind();

    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    fbo->draw(blur_vertical);
  }
};

int main() {
  SSAOExample().run();
}

