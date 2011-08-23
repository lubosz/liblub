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

class DepthBufferExample: public Application {
 public:

   Material * debugfbo, * depthMaterial;
   FrameBuffer * fbo;
   unsigned width, height;

  explicit DepthBufferExample() {
    fontOverlay = false;
    sceneLoader = new SceneLoader("nice.xml");
  }

  ~DepthBufferExample() {}

  void scene() {
    sceneLoader->load();

    SceneData::Instance().name = "Depth Buffer";

    width = SceneData::Instance().width;
    height = SceneData::Instance().height;

    Texture * targetTexture = new DepthTexture(width, height, "result");

    fbo = new FrameBuffer(width, height);

    fbo->attachTexture(GL_DEPTH_ATTACHMENT, targetTexture);
    fbo->disableColorBuffer();
    fbo->checkAndFinish();

    QList<string> attributes;
    attributes.push_back("uv");

    debugfbo = new Simple("Texture/debugfbo",attributes);
    debugfbo->addTexture(targetTexture);

    Material * depthMaterial = new Simple("Common/depth",QList<string>());

  }
  void renderFrame(){
    RenderEngine::Instance().clear();
    fbo->bind();
    RenderEngine::Instance().clear();
    fbo->updateRenderView();
    depthMaterial->activate();
    SceneGraph::Instance().drawCasters(depthMaterial);
    fbo->unBind();
    glViewport(0, 0, width, height);
    fbo->draw(debugfbo);
  }
};

int main(int argc, char *argv[]) {
  DepthBufferExample().run();
}

