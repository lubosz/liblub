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
#include "deferred.h"
#include "Material/Textures.h"
#include "Window/Qt/FloatEditorWidget.h"
#include "Material/Materials.h"
#include "Scene/SceneData.h"
#include "System/TemplateEngine.h"

  DeferredLightApp::DeferredLightApp(int argc, char *argv[]) :
    Application(argc, argv) {
    sceneLoader = new SceneLoader("multilight.xml");
    fontOverlay = false;
  }

  DeferredLightApp::~DeferredLightApp() {
  }

  void DeferredLightApp::scene() {

    QList<string> uv = QList<string> () << "uv";

    sceneLoader->load();
    QSize res = SceneData::Instance().getResolution();

    //
    // shadow passes
    //

    unsigned countLights = 0;
    foreach(Light * light, SceneData::Instance().lights) {
      string name = "shadowDepthSource" + QString::number(countLights).toStdString();
      vector<Texture*> shadowTargets = {
          new ShadowTexture(res, name)
      };
      ShadowCastPass * shadowCastPass = new ShadowCastPass(
          res, shadowTargets, new Minimal(), light
      );
      shadowCastPasses.push_back(shadowCastPass);
      drawPasses.push_back(shadowCastPass);
      countLights++;
    }

    //
    // source pass
    //

    ShadowCastPass * shadowCastPass = shadowCastPasses[2];
    Texture * shadowTarget = shadowCastPass->getTarget("shadowDepthSource2");
    SceneData::Instance().setShadowLight(shadowCastPass->view);

    vector<Texture*> shadowReceiveTargets = {
        new ColorTexture(res, "positionTarget"),
        new ColorTexture(res, "normalTarget"),
        new ColorTexture(res, "diffuseTarget"),
        new ColorTexture(res, "tangentTarget"),
        new ColorTexture(res, "normalMapTarget"),
        new ColorTexture(res, "envTarget"),
        new ColorTexture(res, "shadowTarget"),
        new DepthTexture(res, "depthTarget")
    };


    vector<Texture*> shadowReceiveSources = {
      shadowTarget,
      SceneData::Instance().getTexture("masonry-wall-texture","diffuseTexture"),
      SceneData::Instance().getTexture("masonry-wall-normal-map","normalTexture"),
      SceneData::Instance().getTexture("sky", "envMap")
    };

    TemplateEngine::Instance().c.insert(
        "shadowDepthSource",
        QString::fromStdString(shadowTarget->name)
    );

    SourcePass * shadowReceivePass = new ShadowReceivePass(
        res,
        shadowReceiveSources,
        shadowReceiveTargets,
        new Template("Post/MultiTarget",
            QList<string> () << "uv" << "normal" << "tangent"));
    drawPasses.push_back(shadowReceivePass);

    //
    // ao passes
    //

    vector<Texture*> aoSources = {
        shadowReceivePass->getTarget("normalTarget"),
        shadowReceivePass->getTarget("depthTarget"),
        new TextureFile("noise.png", "noise"),
    };
    vector<Texture*> aoTargets = {
        new ColorTexture(res, "ao")
    };
    InOutPass * aoPass = new InOutPass(res, aoSources, aoTargets,
        new Simple("AO/ssao", uv));
    drawPasses.push_back(aoPass);


    vector<Texture*> mergeSources = {
        aoPass->getTarget("ao"),
        shadowReceivePass->getTarget("shadowTarget"),
    };
    vector<Texture*> mergeTargets = {
        new ColorTexture(res, "merge")
    };

    TemplateEngine::Instance().c.insert("source1", QString::fromStdString(aoPass->getTarget("ao")->name));
    TemplateEngine::Instance().c.insert("source2", QString::fromStdString(shadowReceivePass->getTarget("shadowTarget")->name));
    InOutPass * mergePass = new InOutPass(res, mergeSources, mergeTargets,
        new Template("Post/Merge", uv));
    drawPasses.push_back(mergePass);


    vector<Texture*> blurHSources = {
        mergePass->getTarget("merge"),
    };
    vector<Texture*> blurHTargets = {
        new ColorTexture(res, "blurH")
    };
    InOutPass * blurHPass = new InOutPass(res, blurHSources, blurHTargets,
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
        shadowReceivePass->getTarget("positionTarget"),
        shadowReceivePass->getTarget("normalTarget"),
        shadowReceivePass->getTarget("diffuseTarget"),
        shadowReceivePass->getTarget("tangentTarget"),
        shadowReceivePass->getTarget("normalMapTarget"),
        shadowReceivePass->getTarget("envTarget"),
        blurVPass->getTarget("finalAOTarget"),
//        shadowReceivePass->getTarget("shadowTarget")
    };
    SinkPass * shadingPass = shadingPass = new SinkPass(res, shadingSources,
        new Template("Post/DeferredMultiLight", uv));

    // debug planes
    shadingPass->debugTarget(QRectF(0.5, -1, 0.5, 0.5),
        aoPass->getTarget("ao"));
    shadingPass->debugTarget(QRectF(0.5, -0.5, 0.5, 0.5),
        shadowReceivePass->getTarget("shadowTarget")
    );
    shadingPass->debugTarget(QRectF(0.5, 0, 0.5, 0.5),
        blurVPass->getTarget("finalAOTarget")
     );

    // Init Light Uniform Buffer
    SceneData::Instance().initLightBuffer(
        shadingPass->material->getShaderProgram(), "LightSourceBuffer");

    drawPasses.push_back(shadingPass);
  }

  void DeferredLightApp::renderFrame() {
    foreach(DrawPass * pass, drawPasses)
        pass->draw();
  }

  void DeferredLightApp::setOffSetFactor(double factor) {
    foreach(ShadowCastPass* shadowCastPass, shadowCastPasses)
        shadowCastPass->setOffsetFactor(factor);
  }

  void DeferredLightApp::setOffSetUnits(double units) {
    foreach(ShadowCastPass* shadowCastPass, shadowCastPasses)
        shadowCastPass->setOffsetUnits(units);
  }
#ifdef USE_QT_WINDOWS
  void DeferredLightApp::initWidgets(QHBoxLayout * mainLayout) {
    QVBoxLayout *sideLayout = new QVBoxLayout;
    mainLayout->addLayout(sideLayout);

    QCheckBox *postBox = new QCheckBox();
    postBox->setText("Postprocessing");
    postBox->setChecked(true);
    //      connect(postBox, SIGNAL(clicked(bool)), this, SLOT(setPostprocessing(bool)));
    sideLayout->addWidget(postBox);

    FloatEditorWidget* factor = new FloatEditorWidget("Offset Factor",
        SLOT(setOffSetFactor(double)), 2.0, -25, 25, this);
    connect(factor, SIGNAL(draw()), glWidget, SLOT(updateGL()));
    sideLayout->addWidget(factor);

    FloatEditorWidget* units = new FloatEditorWidget("Offset Units",
        SLOT(setOffSetUnits(double)), 0, -1000, 1000, this);
    connect(units, SIGNAL(draw()), glWidget, SLOT(updateGL()));
    sideLayout->addWidget(units);

    QCheckBox *wireFrameBox = new QCheckBox();
    wireFrameBox->setText("Wireframe");
    wireFrameBox->setChecked(false);
    //      connect(wireFrameBox, SIGNAL(clicked(bool)), this, SLOT(setWireframe(bool)));
    sideLayout->addWidget(wireFrameBox);

    QCheckBox *lazyBox = new QCheckBox();
    lazyBox->setText("Lazy Rendering");
    lazyBox->setChecked(true);
    connect(lazyBox, SIGNAL(clicked(bool)), this, SLOT(setLazy(bool)));
    sideLayout->addWidget(lazyBox);
  }
#endif

int main(int argc, char *argv[]) {
  DeferredLightApp(argc, argv).run();
  return 0;
}

