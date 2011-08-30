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

    QList<string> uv = QList<string> () << "uv";

    sceneLoader->load();
    QSize res = SceneData::Instance().getResolution();

    //
    // shadow pass
    //

    vector<Texture*> shadowTargets = {
        new ShadowTexture(res, "shadowDepthSource")
    };
    SourcePass * shadowPass = new ShadowCastPass(
        res, shadowTargets, new Minimal(), SceneData::Instance().getShadowLight()
    );
    drawPasses.push_back(shadowPass);
    //
    // source pass
    //

    vector<Texture*> sourceTargets = {
        new ColorTexture(res, "positionTarget"),
        new ColorTexture(res, "normalTarget"),
        new ColorTexture(res, "diffuseTarget"),
        new ColorTexture(res, "tangentTarget"),
        new ColorTexture(res, "normalMapTarget"),
        new ColorTexture(res, "envTarget"),
        new ColorTexture(res, "shadowTarget"),
        new DepthTexture(res, "depthTarget")
    };

    vector<Texture*> sourceTextures = {
      SceneData::Instance().getTexture("masonry-wall-texture","diffuseTexture"),
      SceneData::Instance().getTexture("masonry-wall-normal-map","normalTexture"),
      SceneData::Instance().getTexture("sky", "envMap"),
      shadowPass->getTarget("shadowDepthSource")
    };

    SourcePass * sourcePass = new ShadowReceivePass(
        res,
        sourceTargets,
        new Template("Post/MultiTarget",
            QList<string> () << "uv" << "normal" << "tangent"));
    sourcePass->addTextures(sourceTextures);
    drawPasses.push_back(sourcePass);

    //
    // ao passes
    //

    vector<Texture*> aoSources = {
        sourcePass->getTarget("normalTarget"),
        sourcePass->getTarget("depthTarget")
    };
    vector<Texture*> aoTargets = {
        new ColorTexture(res, "ao")
    };
    InOutPass * aoPass = new InOutPass(res, aoSources, aoTargets,
        new Simple("AO/ssao", uv));
    aoPass->addTexture(new TextureFile("noise.png", "noise"));
    drawPasses.push_back(aoPass);

    vector<Texture*> blurHTargets = {
        new ColorTexture(res, "blurH")
    };
    InOutPass * blurHPass = new InOutPass(res, aoTargets, blurHTargets,
        new Simple("AO/blur_horizontal", uv));
    drawPasses.push_back(blurHPass);

    vector<Texture*> blurVTargets = {
        new ColorTexture(res, "finalAOTarget")
    };
    InOutPass * blurVPass = new InOutPass(res, blurHTargets, blurVTargets,
        new Simple("AO/blur_vertical", uv));
    drawPasses.push_back(blurVPass);

    //
    // shading pass
    //

    vector<Texture*> shadingSources = {
        sourcePass->getTarget("positionTarget"),
        sourcePass->getTarget("normalTarget"),
        sourcePass->getTarget("diffuseTarget"),
        sourcePass->getTarget("tangentTarget"),
        sourcePass->getTarget("normalMapTarget"),
        sourcePass->getTarget("envTarget"),
        blurVPass->getTarget("finalAOTarget"),
        sourcePass->getTarget("shadowTarget")
    };
    SinkPass * shadingPass = shadingPass = new SinkPass(res, shadingSources,
        new Template("Post/DeferredMultiLight", uv));
    shadingPass->debugTarget(QRectF(0.5, -1, 0.5, 0.5),
        sourcePass->getTarget("normalTarget"));
    shadingPass->debugTarget(QRectF(0.5, -0.5, 0.5, 0.5),
//        sourcePass->getTarget("diffuseTarget")
        sourcePass->getTarget("shadowTarget")
    );
    shadingPass->debugTarget(QRectF(0.5, 0, 0.5, 0.5),
        blurVPass->getTarget("finalAOTarget")
     );

    // Init Light Uniform Buffer
    SceneData::Instance().initLightBuffer(
        shadingPass->material->getShaderProgram(), "LightSourceBuffer");

    drawPasses.push_back(shadingPass);
  }

  void renderFrame() {
    foreach(DrawPass * pass, drawPasses)
        pass->draw();
  }

  void initWidgets(QHBoxLayout * mainLayout) {}
};

int main(int argc, char *argv[]) {
  DeferredLightApp(argc, argv).run();
  return 0;
}

