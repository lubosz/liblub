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

#include "planet-editor.h"

#include "Scene/Scene.h"
#include "Texture/Textures.h"
#include "Load/AssimpSceneLoader.h"
#include "Load/SceneLoader.h"
#include "Renderer/DeferredRenderer.h"
#include "Procedural/Geometry.h"
#include "Scene/SceneGraph.h"
#include "Application/Qt/FloatEditorWidget.h"

#include <QSplitter>

PlanetEditor::PlanetEditor(int &argc, char **argv) :
    QtApplication(argc, argv) {
    demo = new PlanetsDemo();
}

PlanetEditor::~PlanetEditor() {
}


PlanetWidget * PlanetEditor::focusPlanet() {
  PlanetWidget * planetWidget = new PlanetWidget(demo->focusedPlanet);
  connect(planetWidget, SIGNAL(draw(void)), glWidget, SLOT(updateGL()));
  return planetWidget;
}

void PlanetEditor::setPostprocessing(bool post) {
  demo->usePostprocessing = post;
  glWidget->updateGL();
}

void PlanetEditor::setExposure(double exposure) {
  demo->HDR->use();
  demo->HDR->setUniform("exposure", static_cast<float>(exposure));
  glWidget->updateGL();
}

void PlanetEditor::initWidgets(QSplitter *mainSplitter) {
  QVBoxLayout *sideLayout = new QVBoxLayout;

  QWidget *sideLayoutWidget = new QWidget;
  sideLayoutWidget->setLayout(sideLayout);
  mainSplitter->addWidget(sideLayoutWidget);

  QListWidget * planetList = new QListWidget();

  foreach(Planet * planet , demo->planets) {
      QListWidgetItem * planetItem = new QListWidgetItem(planetList);
      planetItem->setText(planet->name);
  }
  QCheckBox *postBox = new QCheckBox();
  postBox->setText("Postprocessing");
  postBox->setChecked(true);
  connect(postBox, SIGNAL(clicked(bool)), this, SLOT(setPostprocessing(bool)));
  sideLayout->addWidget(postBox);

  FloatEditorWidget* exposureWidget = new FloatEditorWidget("Exposure", SLOT(setExposure(double)), 2.0, 0, 10, this);
  connect(exposureWidget, SIGNAL(draw()), glWidget, SLOT(updateGL()));
  sideLayout->addWidget(exposureWidget);

  QCheckBox *wireFrameBox = new QCheckBox();
  wireFrameBox->setText("Wireframe");
  wireFrameBox->setChecked(false);
  connect(wireFrameBox, SIGNAL(clicked(bool)), this, SLOT(setWireframe(bool)));
  sideLayout->addWidget(wireFrameBox);

  QCheckBox *lazyBox = new QCheckBox();
  lazyBox->setText("Lazy Rendering");
  lazyBox->setChecked(true);
  connect(lazyBox, SIGNAL(clicked(bool)), this, SLOT(setLazy(bool)));
  sideLayout->addWidget(lazyBox);

  sideLayout->addWidget(focusPlanet());
}

void PlanetEditor::scene() {
      window->setWindowTitle("Planets Demo");
      demo->init();
      initWidgets(window->splitter);
}

void PlanetEditor::renderFrame() {
    demo->draw();
}

int main(int argc, char **argv) {
      PlanetEditor(argc, argv).run();
}

