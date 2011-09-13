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
#include <QtGui>
#include "shadow.h"
#include "Material/Textures.h"
#include "Window/Qt/FloatEditorWidget.h"
#include "Material/Materials.h"
#include "Scene/SceneData.h"
#include "System/TemplateEngine.h"

  ShadowApp::ShadowApp(int argc, char *argv[]) :
    Application(argc, argv) {
    sceneLoader = new SceneLoader("multilight.xml");
    fontOverlay = false;
  }

  ShadowApp::~ShadowApp() {
  }

  void ShadowApp::scene() {

    QList<string> uv = QList<string> () << "uv";

    sceneLoader->load();
    QSize res = SceneData::Instance().getResolution();

    //
    // shadow passes
    //

    unsigned countLights = 0;
    foreach(Light * light, SceneData::Instance().lights) {
      string name = "shadowDepthSource" + QString::number(countLights).toStdString();
      vector<Texture*> shadowSources = {new ShadowTexture(res, name)};
      ShadowCastPass * shadowCastPass = new ShadowCastPass(
          res, shadowSources, light
      );
      shadowCastPasses.push_back(shadowCastPass);
      drawPasses.push_back(shadowCastPass);
      countLights++;
    }

    //
    // source pass
    //

    vector<Texture*> shadowReceiveTargets = {
        new ColorTexture(res, "positionTarget"),
        new ColorTexture(res, "normalTarget"),
        new ColorTexture(res, "diffuseTarget"),
        new ColorTexture(res, "tangentTarget"),
        new ColorTexture(res, "normalMapTarget"),
        new ColorTexture(res, "shadowTarget"),
        new ColorTexture(res, "reflectionTarget")
    };

    vector<Texture*> shadowReceiveSources = {
      SceneData::Instance().getTexture("masonry-wall-normal-map","normalTexture"),
      SceneData::Instance().getTexture("masonry-wall-texture","diffuseTexture"),
      SceneData::Instance().getTexture("sky", "envMap")
    };

    foreach(ShadowCastPass * shadowCastPass, shadowCastPasses) {
      shadowReceiveSources.push_back(shadowCastPass->targets[0]);
    }

    // set sampler "array" via template engine

    QStringList shadowSamplers;
    foreach(ShadowCastPass * shadowCastPass, shadowCastPasses) {
      shadowSamplers <<
          QString::fromStdString(shadowCastPass->targets[0]->name);
    }

    TemplateEngine::Instance().c.insert("shadowSamplers", shadowSamplers);
    TemplateEngine::Instance().c.insert("shadowSamplerSize", shadowSamplers.size());


    SourcePass * shadowReceivePass = new ShadowReceivePass(
        res,
        shadowReceiveSources,
        shadowReceiveTargets,
        new Template("Post/MultiTarget",
            QList<string> () << "uv" << "normal" << "tangent"));
    drawPasses.push_back(shadowReceivePass);


    SinkPass * sinkPass = new SinkPass();

    sinkPass->debugTarget(QRectF(-1, -1, 2, 2),
        shadowReceivePass->getTarget("shadowTarget")
    );
    drawPasses.push_back(sinkPass);
  }

  void ShadowApp::renderFrame() {
    foreach(DrawThing * pass, drawPasses)
        pass->draw();
  }

int main(int argc, char *argv[]) {
  ShadowApp(argc, argv).run();
  return 0;
}

