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

#include "editor.h"

#include "Scene/Scene.h"
#include "Material/Textures.h"
#include "Scene/AssimpSceneLoader.h"
#include "Renderer/DeferredRenderer.h"
#include "Mesh/Geometry.h"
#include "Scene/SceneGraph.h"
#include "TextureModel.h"
#include "Nodes/GraphWidget.h"
#include "Scene/SceneLoader.h"
#include "Window/Qt/FloatEditorWidget.h"

Editor::Editor(int &argc, char **argv) :
    Application(argc, argv) {
    scenePath = argv[1];
    transparencyModes.insert("GL_ZERO", GL_ZERO);
    transparencyModes.insert("GL_ONE", GL_ONE);

    transparencyModes.insert("GL_SRC_COLOR", GL_SRC_COLOR);
    transparencyModes.insert("GL_ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR);

    transparencyModes.insert("GL_DST_COLOR", GL_DST_COLOR);
    transparencyModes.insert("GL_ONE_MINUS_DST_COLOR", GL_ONE_MINUS_DST_COLOR);

    transparencyModes.insert("GL_SRC_ALPHA", GL_SRC_ALPHA);
    transparencyModes.insert("GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA);

    transparencyModes.insert("GL_DST_ALPHA", GL_DST_ALPHA);
    transparencyModes.insert("GL_ONE_MINUS_DST_ALPHA", GL_ONE_MINUS_DST_ALPHA);

    transparencyModes.insert("GL_CONSTANT_COLOR", GL_CONSTANT_COLOR);
    transparencyModes.insert("GL_ONE_MINUS_CONSTANT_COLOR", GL_ONE_MINUS_CONSTANT_COLOR);

    transparencyModes.insert("GL_CONSTANT_ALPHA", GL_CONSTANT_ALPHA);
    transparencyModes.insert("GL_ONE_MINUS_CONSTANT_ALPHA", GL_ONE_MINUS_CONSTANT_ALPHA);

    transparencyModes.insert("GL_SRC_ALPHA_SATURATE", GL_SRC_ALPHA_SATURATE);
}

Editor::~Editor() {
}

void Editor::setOffSetFactor(double factor) {
  foreach(ShadowCastPass* shadowCastPass, DeferredRenderer::Instance().shadowCastPasses)
      shadowCastPass->setOffsetFactor(factor);
}

void Editor::setOffSetUnits(double units) {
  foreach(ShadowCastPass* shadowCastPass, DeferredRenderer::Instance().shadowCastPasses)
      shadowCastPass->setOffsetUnits(units);
}

void Editor::scene() {
    Scene::Instance().getCurrentCamera()->setPosition(QVector3D(-1.43765, 5.130675, -1.20157));
    Scene::Instance().getCurrentCamera()->setDirection(QVector3D(0.741701, -0.0836778, 0.66549));
    Scene::Instance().getCurrentCamera()->update();

    DeferredRenderer::Instance().initSky("cubemaps/sky");

    QString qScenePath = QString::fromStdString(scenePath);

    if (qScenePath.contains(".xml")) {
        SceneLoader * sceneLoader = new SceneLoader(qScenePath);
        sceneLoader->load();
    } else{
        AssimpSceneLoader::Instance().load(scenePath);
    }

    DeferredRenderer::Instance().init();
    initWidgets(window->splitter);

}
void Editor::renderFrame() {
    DeferredRenderer::Instance().draw();
}
void Editor::setSelectedPlane(const QModelIndex &index) {
    selectedPlane = DeferredRenderer::Instance().sinkPass->debugPlanes[index.row()];

    vector<string> targets = DeferredRenderer::Instance().getTargetNames();
    for (unsigned i = 0; i < targets.size(); ++i) {
        if (targets[i] == selectedPlane->targetName)
            renderTargetSelector->setCurrentIndex(i);
    }
}

void Editor::changePlaneSource(const QString &name) {
    DeferredRenderer::Instance().changePlaneSource(selectedPlane, name);
    glWidget->updateGL();
}

void Editor::setSelectedTexture(const QModelIndex &index) {
    Texture * texture = Scene::Instance().textures.values().at(index.row());
    selectedPlane->updateSource(texture);
    glWidget->updateGL();
}

void Editor::setSelectedPass(const QModelIndex &index) {
    DrawThing * pass = DeferredRenderer::Instance().drawPasses.at(index.row());

    QLayoutItem* item;
    while ((item = passLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
    }

    SourcePass * sourceCheck = dynamic_cast<SourcePass*>(pass);
    if (sourceCheck != nullptr) {
        QLabel * targetLabel = new QLabel;
        targetLabel->setText("<b>Targets</b>");
        passLayout->addWidget(targetLabel);

        foreach(Texture * target, sourceCheck->targets) {
            QLabel * label = new QLabel;
            label->setText(QString::fromStdString(target->name));
            passLayout->addWidget(label);
        }
    }

    InOutPass * inOutCheck = dynamic_cast<InOutPass*>(pass);
    if (inOutCheck != nullptr) {

        QLabel * sourceLabel = new QLabel;
        sourceLabel->setText("<b>Sources</b>");
        passLayout->addWidget(sourceLabel);

        foreach(Texture * target, inOutCheck->sources) {
            QLabel * label = new QLabel;
            label->setText(QString::fromStdString(target->name));
            passLayout->addWidget(label);
        }

    }

//    SinkPass * sinkCheck = dynamic_cast<SinkPass*>(pass);
//    if (sinkCheck != nullptr)
//        passType = "SinkPass";
}


void Editor::setTransparency(bool transparent) {
  DeferredRenderer::Instance().drawTransparency = transparent;
  glWidget->updateGL();
}

void Editor::updateTransparency() {
    glBlendFunc(srcMode, destMode);
    glWidget->updateGL();
}

void Editor::setSelectedTransparencySrc(const QString &srcModeName) {
    srcMode = transparencyModes[srcModeName];
//    LogDebug << "Src Mode" << srcModeName.toStdString() << srcMode;
    updateTransparency();
}

void Editor::setSelectedTransparencyDest(const QString &destModeName) {
    destMode = transparencyModes[destModeName];
//    LogDebug << "Dest Mode" << destModeName.toStdString() << destMode;
    updateTransparency();
}

void Editor::initWidgets(QSplitter * mainSplitter) {
    QVBoxLayout *sideLayout = new QVBoxLayout;

    QWidget *sideLayoutWidget = new QWidget;
    sideLayoutWidget->setLayout(sideLayout);
    mainSplitter->addWidget(sideLayoutWidget);

    QCheckBox *lazyBox = new QCheckBox();
    lazyBox->setText("Lazy Rendering");
    lazyBox->setChecked(true);
    connect(lazyBox, SIGNAL(clicked(bool)), this, SLOT(setLazy(bool)));
    sideLayout->addWidget(lazyBox);

    QCheckBox *transparencyBox = new QCheckBox();
    transparencyBox->setText("Draw Transparency");
    transparencyBox->setChecked(true);
    connect(transparencyBox, SIGNAL(clicked(bool)), this, SLOT(setTransparency(bool)));
    sideLayout->addWidget(transparencyBox);

    QCheckBox *wireFrameBox = new QCheckBox();
    wireFrameBox->setText("Wireframe");
    wireFrameBox->setChecked(false);
    //      connect(wireFrameBox, SIGNAL(clicked(bool)), this, SLOT(setWireframe(bool)));
    sideLayout->addWidget(wireFrameBox);


    QTabWidget * tabWidget = new QTabWidget;
    sideLayout->addWidget(tabWidget);

    //target tab
    QWidget * renderTargetTab = new QWidget();
    tabWidget->addTab(renderTargetTab, "Targets");


    QVBoxLayout *renderTargetLayout = new QVBoxLayout(renderTargetTab);

    QListView *targetListView = new QListView;
    targetModel = new TargetModel(0);
    connect(targetModel, SIGNAL(draw()), glWidget, SLOT(updateGL()));
    targetListView->setModel(targetModel);
    targetListView->show();
    renderTargetLayout->addWidget(targetListView);


    QListView *passListView = new QListView;
    passModel = new PassModel(0);
    connect(passModel, SIGNAL(draw()), glWidget, SLOT(updateGL()));
    passListView->setModel(passModel);
    passListView->show();
    renderTargetLayout->addWidget(passListView);


    passLayout = new QVBoxLayout;
    passLayoutWidget = new QWidget;
    passLayoutWidget->setLayout(passLayout);
    renderTargetLayout->addWidget(passLayoutWidget);

    renderTargetSelector = new QComboBox;

    vector<string> targets = DeferredRenderer::Instance().getTargetNames();

    for (unsigned i = 0; i < targets.size(); ++i) {
        renderTargetSelector->insertItem(i, QString::fromStdString(targets[i]));
    }
    renderTargetLayout->addWidget(renderTargetSelector);

    QWidget * textureTab = new QWidget();
    tabWidget->addTab(textureTab, "Textures");
    QVBoxLayout *textureTabLayout = new QVBoxLayout(textureTab);

    QTreeView *texturelistView = new QTreeView;
    TextureModel * texModel = new TextureModel(0);
    texturelistView->setModel(texModel);
    texturelistView->show();

    texturelistView->resizeColumnToContents(2);

    textureTabLayout->addWidget(texturelistView);

    transparencyModeSrc = new QComboBox;
    transparencyModeDest = new QComboBox;
    unsigned index = 0;
    foreach (QString mode, transparencyModes.keys()) {
        transparencyModeSrc->insertItem(index, mode);
        transparencyModeDest->insertItem(index, mode);
        index++;
    }
    connect(transparencyModeSrc, SIGNAL(currentIndexChanged(QString)), this, SLOT(setSelectedTransparencySrc(QString)));
    connect(transparencyModeDest, SIGNAL(currentIndexChanged(QString)), this, SLOT(setSelectedTransparencyDest(QString)));

    QWidget * transparencyBoxLayoutWidget = new QWidget;
    QVBoxLayout * transparencyBoxLayout = new QVBoxLayout(transparencyBoxLayoutWidget);
    transparencyBoxLayout->addWidget(transparencyModeSrc);
    transparencyBoxLayout->addWidget(transparencyModeDest);

    renderTargetLayout->addWidget(transparencyBoxLayoutWidget);

    setSelectedPlane(targetModel->index(0, 0, QModelIndex()));
    connect(targetListView, SIGNAL(clicked(QModelIndex)), this, SLOT(setSelectedPlane(QModelIndex)));
    connect(passListView, SIGNAL(clicked(QModelIndex)), this, SLOT(setSelectedPass(QModelIndex)));
    connect(texturelistView, SIGNAL(clicked(QModelIndex)), this, SLOT(setSelectedTexture(QModelIndex)));
    connect(renderTargetSelector, SIGNAL(currentIndexChanged(QString)), this, SLOT(changePlaneSource(QString)));
    connect(renderTargetSelector, SIGNAL(currentIndexChanged(QString)), passListView, SLOT(updateGeometries()));

    GraphWidget * graphWidget = new GraphWidget;
    tabWidget->addTab(graphWidget, "Nodes");


    // Shadow Tab
    QWidget * shadowTab = new QWidget();
    tabWidget->addTab(shadowTab, "Shadow");


    QVBoxLayout *shadowTabLayout = new QVBoxLayout(shadowTab);


    FloatEditorWidget* factor = new FloatEditorWidget("Offset Factor",
        SLOT(setOffSetFactor(double)), 2.0, -25, 25, this);
    connect(factor, SIGNAL(draw()), glWidget, SLOT(updateGL()));
    shadowTabLayout->addWidget(factor);

    FloatEditorWidget* units = new FloatEditorWidget("Offset Units",
        SLOT(setOffSetUnits(double)), 0, -1000, 1000, this);
    connect(units, SIGNAL(draw()), glWidget, SLOT(updateGL()));
    shadowTabLayout->addWidget(units);
}

int main(int argc, char **argv) {
    if (argc != 2)
      LogError << "NO SCENE SPECIFIED. Try; ./editor foo.blend";
    else
      Editor(argc, argv).run();
}

