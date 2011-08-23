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

class SSAOApp: public Application {
 public:

   RenderPass * defaultPass;
   Material * debugfbo;

  explicit SSAOApp() {
    fontOverlay = false;
    sceneLoader = new SceneLoader("nice.xml");
  }

  ~SSAOApp() {}

  void scene() {
    sceneLoader->load();
    SceneData::Instance().name = "SSAO";
    unsigned width = SceneData::Instance().width;
    unsigned height = SceneData::Instance().height;
    FrameBuffer * fbo = new FrameBuffer(width, height);

//    Texture * targetTexture = new DepthTexture(width, height, "shadowMap");
    Texture * targetTexture = new ColorTexture(width, height, "shadowMap");

    QList<string> attributes;
    attributes.push_back("uv");

    debugfbo = new Simple("Texture/debugfbo",attributes);
//    debugfbo = new Simple("Post/ssao",attributes);
    Material * depthMaterial = new Simple("Common/depth",QList<string>());
    defaultPass = new WritePass(fbo, targetTexture, depthMaterial, true);
    debugfbo->addTexture(defaultPass->targetTexture);
    fbo->checkAndFinish();
  }
  void renderFrame(){
    RenderEngine::Instance().clear();
    defaultPass->render();
    defaultPass->fbo->draw(debugfbo);
  }
};

int main(int argc, char *argv[]) {
  SSAOApp().run();
}

