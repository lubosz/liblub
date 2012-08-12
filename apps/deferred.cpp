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
#include "Window/Qt/FloatEditorWidget.h"
#include "Scene/Scene.h"
#include "Renderer/DeferredRenderer.h"

  DeferredLightApp::DeferredLightApp(int argc, char *argv[]) :
    Application(argc, argv) {
    sceneLoader = new SceneLoader("multilight.xml");
    fontOverlay = false;
  }

  DeferredLightApp::~DeferredLightApp() {
  }

  void DeferredLightApp::scene() {
#ifdef USE_QT_WINDOWS
      initWidgets(window->mainLayout);
#endif
      //      glEnable(GL_CULL_FACE);
//      glCullFace(GL_BACK);
      glDepthFunc(GL_LEQUAL);

    sceneLoader->load();

    Scene::Instance().getCurrentCamera()->setPosition(QVector3D(-1.43765, 0.130675, -1.20157));
    Scene::Instance().getCurrentCamera()->setDirection(QVector3D(0.741701, -0.0836778, 0.66549));
    Scene::Instance().getCurrentCamera()->update();

    DeferredRenderer::Instance().init();
  }

  void DeferredLightApp::renderFrame() {
      DeferredRenderer::Instance().draw();
  }

  void DeferredLightApp::setOffSetFactor(double factor) {
    foreach(ShadowCastPass* shadowCastPass, DeferredRenderer::Instance().shadowCastPasses)
        shadowCastPass->setOffsetFactor(factor);
  }

  void DeferredLightApp::setOffSetUnits(double units) {
    foreach(ShadowCastPass* shadowCastPass, DeferredRenderer::Instance().shadowCastPasses)
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
