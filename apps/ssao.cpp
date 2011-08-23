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

   Material * debugfbo, * depthMaterial;
   FrameBuffer * fbo;
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

    Texture * targetTexture = new ColorTexture(res, "result");

    fbo = new FrameBuffer(res);

    fbo->attachTexture(GL_COLOR_ATTACHMENT0, targetTexture);
    fbo->check();

    QList<string> attributes;
    attributes.push_back("uv");

    debugfbo = new Simple("Texture/debugfbo",attributes);
    debugfbo->addTexture(targetTexture);

    depthMaterial = new Simple("Common/depth",QList<string>());

  }
  void renderFrame(){
    RenderEngine::Instance().clear();
    fbo->bind();
    RenderEngine::Instance().clear();
    fbo->updateRenderView();
    depthMaterial->activate();
    SceneGraph::Instance().drawCasters(depthMaterial);
    fbo->unBind();
    RenderEngine::Instance().updateViewport(res);
    fbo->draw(debugfbo);
  }
};

int main(int argc, char *argv[]) {
  SSAOExample().run();
}

