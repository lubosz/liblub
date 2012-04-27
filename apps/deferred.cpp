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
#include "Material/Shaders.h"
#include "Scene/Scene.h"
#include "System/TemplateEngine.h"
#include "Scene/InstancedSponge.h"

  DeferredLightApp::DeferredLightApp(int argc, char *argv[]) :
    Application(argc, argv) {
    sceneLoader = new SceneLoader("multilight.xml");
    fontOverlay = false;
  }

  DeferredLightApp::~DeferredLightApp() {
  }

  void DeferredLightApp::scene() {
//      glEnable(GL_CULL_FACE);
//      glCullFace(GL_BACK);
      glDepthFunc(GL_LEQUAL);
    QList<string> uv = QList<string> () << "uv";

    sceneLoader->load();
    QSize res = Scene::Instance().getResolution();

    Scene::Instance().getCurrentCamera()->setPosition(QVector3D(-1.43765, 0.130675, -1.20157));
    Scene::Instance().getCurrentCamera()->setDirection(QVector3D(0.741701, -0.0836778, 0.66549));
    Scene::Instance().getCurrentCamera()->update();

    //
    // shadow passes
    //

    unsigned countLights = 0;
    foreach(Light * light, Scene::Instance().lights) {
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
        //new ColorTexture(res, "diffuseTarget"),
        new ColorTexture(res, "tangentTarget"),
        new ColorTexture(res, "binormalTarget"),
        //new ColorTexture(res, "normalMapTarget"),
        new ColorTexture(res, "shadowTarget"),
        new DepthTexture(res, "depthTarget"),
        new ColorTexture(res, "uvTarget"),
    };

    vector<Texture*> shadowReceiveSources = {
//      SceneData::Instance().getTexture("masonry-wall-normal-map","normalTexture"),
//      SceneData::Instance().getTexture("masonry-wall-texture","diffuseTexture"),
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

    QList<string> tangent = QList<string> () << "uv" << "normal" << "tangent" << "bitangent";
//    InstancedSponge *sponge = new InstancedSponge(2, tangent);

    TemplateEngine::Instance().c.insert("shadowSamplers", shadowSamplers);
    TemplateEngine::Instance().c.insert("shadowSamplerSize", shadowSamplers.size());
//    TemplateEngine::Instance().c.insert("positionElements", QVariant::fromValue(sponge->positionBufferDataSize));
//    TemplateEngine::Instance().c.insert("isInstanced", true);
    ShaderProgram * gatherShader = new TemplateProgram("Post/MultiTarget",tangent);


    SourcePass * shadowReceivePass = new ShadowReceivePass(
        res,shadowReceiveSources,
        shadowReceiveTargets,gatherShader);

//    sponge->initBuffers(gatherMaterial);
//  SceneGraph::Instance().addNode(sponge);

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
        new SimpleProgram("AO/ssao", uv));
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
        new TemplateProgram("Post/Merge", uv));
    drawPasses.push_back(mergePass);

    vector<Texture*> blurHSources = {
        mergePass->getTarget("merge"),
    };
    vector<Texture*> blurHTargets = {
        new ColorTexture(res, "blurH")
    };
    InOutPass * blurHPass = new InOutPass(res, blurHSources, blurHTargets,
        new SimpleProgram("AO/blur_horizontal", uv));
    drawPasses.push_back(blurHPass);

    vector<Texture*> blurVTargets = {
        new ColorTexture(res, "finalAOTarget")
    };
    InOutPass * blurVPass = new InOutPass(res, blurHTargets, blurVTargets,
        new SimpleProgram("AO/blur_vertical", uv));
    drawPasses.push_back(blurVPass);

    //
    // shading pass
    //

    vector<Texture*> shadingSources = {
        shadowReceivePass->getTarget("positionTarget"),
        shadowReceivePass->getTarget("normalTarget"),
        //shadowReceivePass->getTarget("diffuseTarget"),
        shadowReceivePass->getTarget("tangentTarget"),
        shadowReceivePass->getTarget("binormalTarget"),
        //shadowReceivePass->getTarget("normalMapTarget"),
        blurVPass->getTarget("finalAOTarget"),
        Scene::Instance().getTexture("sky", "envMap"),
        shadowReceivePass->getTarget("uvTarget"),
        Scene::Instance().getTexture("masonry-wall-normal-map","normalTexture"),
        Scene::Instance().getTexture("masonry-wall-texture","diffuseTexture"),
    };

    vector<Texture*> shadingTargets = {
        new ColorTexture(res, "finalTarget"),
        new ColorTexture(res, "finalSpecularTarget"),
        new ColorTexture(res, "finalDiffuseTarget"),
        new ColorTexture(res, "envTarget")
    };

    TemplateEngine::Instance().c.insert("lightCount", Scene::Instance().lights.size());
//    TemplateEngine::Instance().c.insert("paralaxMap", true);
    InOutPass * shadingPass = new InOutPass(res, shadingSources, shadingTargets,
        new TemplateProgram("Post/DeferredMultiLight", uv));
    drawPasses.push_back(shadingPass);

    SinkPass * sinkPass = new SinkPass();

    // debug planes
    sinkPass->debugTarget(QRectF(-1, -1, 2, 2),
        shadingPass->getTarget("finalTarget")
     );
    sinkPass->debugTarget(QRectF(0.5, -1, 0.5, 0.5),
        aoPass->getTarget("ao"));
    sinkPass->debugTarget(QRectF(0.5, -0.5, 0.5, 0.5),
        shadowReceivePass->getTarget("shadowTarget")
    );
    sinkPass->debugTarget(QRectF(0.5, 0, 0.5, 0.5),
        blurVPass->getTarget("finalAOTarget")
     );
    sinkPass->debugTarget(QRectF(-1, 0, 0.5, 0.5),
        shadowReceivePass->getTarget("normalTarget")
     );
    sinkPass->debugTarget(QRectF(-1, -0.5, 0.5, 0.5),
        shadingPass->getTarget("envTarget")
     );
    sinkPass->debugTarget(QRectF(-1, -1, 0.5, 0.5),
        shadingPass->getTarget("finalSpecularTarget")
     );
    sinkPass->debugTarget(QRectF(-1, 0.5, 0.5, 0.5),
        shadingPass->getTarget("finalDiffuseTarget")
     );

    drawPasses.push_back(sinkPass);
    // Init Light Uniform Buffer
    initLightBuffer(shadingPass->shader, "LightSourceBuffer");
  }

  void DeferredLightApp::renderFrame() {
    foreach(DrawThing * pass, drawPasses)
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

void DeferredLightApp::initLightBuffer(ShaderProgram * shader, const string& bufferName) {
  lightBuffer = new UniformBuffer();
  lightBuffer->bind();

  #ifdef USE_OPENGL3
  GLuint uniBlockIndex = glGetUniformBlockIndex(shader->getHandle(), bufferName.c_str());
  glGetActiveUniformBlockiv(
    shader->getHandle(),
    uniBlockIndex,
    GL_UNIFORM_BLOCK_DATA_SIZE,
    &lightBufferSize
  );
#endif

  LogDebug << "Light Uniform Buffer Size" << lightBufferSize;

  unsigned lightIndex = 0;
  foreach(Light* light, Scene::Instance().lights){

    lightBufferData[lightIndex].position = light->position;
    lightBufferData[lightIndex].diffuse = light->diffuse;
    lightBufferData[lightIndex].specular = light->specular;
    lightBufferData[lightIndex].direction = light->direction();

    LogDebug << "Found Light" << Scene::Instance().lights.key(light);
    qDebug() << lightBufferData[lightIndex].diffuse;

    lightIndex++;
    glError;
  }
  lightBuffer->write(lightBufferData, lightBufferSize);

//  shader->uniformBuffers.push_back(lightBuffer);
  #ifdef USE_OPENGL3
  shader->bindUniformBuffer(bufferName,0,lightBuffer->getHandle());
    #endif
}

int main(int argc, char *argv[]) {
  DeferredLightApp(argc, argv).run();
  return 0;
}
