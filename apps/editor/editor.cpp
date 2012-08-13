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

#include "System/Logger.h"
#include "Material/Shaders.h"
#include "Material/Textures.h"
#include "Scene/SceneGraph.h"
#include "Renderer/RenderPasses.h"
#include "Renderer/DeferredRenderer.h"
#include "Scene/Scene.h"
#include "Scene/BlenderLoader.h"

#include <QTableWidget>
#include <QHBoxLayout>
#include <QListView>
#include <QTableView>
#include "System/Config.h"
#include "Window/Qt/FloatEditorWidget.h"
#include "TextureModel.h"
#include <QTreeWidget>

#include <QSplitter>

Editor::Editor(int argc, char *argv[]) :
    Application(argc, argv) {
}

Editor::~Editor() {
}

void Editor::scene() {
    Scene::Instance().getCurrentCamera()->setPosition(QVector3D(-1.43765, 5.130675, -1.20157));
    Scene::Instance().getCurrentCamera()->setDirection(QVector3D(0.741701, -0.0836778, 0.66549));
    Scene::Instance().getCurrentCamera()->update();

    Texture* env = new CubeTextureFile("cubemaps/sky", "sky");

//    Scene::Instance().textures.insert("sky", env);

    BlenderLoader::Instance().load("nature.blend");
    DeferredRenderer::Instance().init();
    initWidgets(window->splitter);

}
void Editor::renderFrame() {
    DeferredRenderer::Instance().draw();
}

void Editor::setSelectedPlane(const QModelIndex &index) {
    selectedPlane = DeferredRenderer::Instance().sinkPass->debugPlanes[index.row()];

    vector<string> targets = DeferredRenderer::Instance().getTargetNames();
    for (int i = 0; i < targets.size(); ++i) {
        if (targets[i] == selectedPlane->targetName)
            renderPassSelector->setCurrentIndex(i);
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


void Editor::initWidgets(QSplitter * mainSplitter) {
    QVBoxLayout *sideLayout = new QVBoxLayout;
//    mainLayout->addL(sideLayout);

    QWidget *sideLayoutWidget = new QWidget;
    sideLayoutWidget->setLayout(sideLayout);
    mainSplitter->addWidget(sideLayoutWidget);

    QCheckBox *lazyBox = new QCheckBox();
    lazyBox->setText("Lazy Rendering");
    lazyBox->setChecked(true);
    connect(lazyBox, SIGNAL(clicked(bool)), this, SLOT(setLazy(bool)));
    sideLayout->addWidget(lazyBox);


    QTabWidget * tabWidget = new QTabWidget;
    sideLayout->addWidget(tabWidget);
    QWidget * renderPassTab = new QWidget();
    tabWidget->addTab(renderPassTab, "Passes");


    QVBoxLayout *renderPassTabLayout = new QVBoxLayout(renderPassTab);

    QListView *passListView = new QListView;
    passModel = new PassModel(0);
    connect(passModel, SIGNAL(draw()), glWidget, SLOT(updateGL()));
    passListView->setModel(passModel);
    passListView->show();
    renderPassTabLayout->addWidget(passListView);


    renderPassSelector = new QComboBox;

    vector<string> targets = DeferredRenderer::Instance().getTargetNames();

    for (int i = 0; i < targets.size(); ++i) {
        renderPassSelector->insertItem(i, QString::fromStdString(targets[i]));
    }
    renderPassTabLayout->addWidget(renderPassSelector);


    QWidget * textureTab = new QWidget();
    tabWidget->addTab(textureTab, "Textures");
    QVBoxLayout *textureTabLayout = new QVBoxLayout(textureTab);

    QTreeView *texturelistView = new QTreeView;
    TextureModel * texModel = new TextureModel(0);
    texturelistView->setModel(texModel);
    texturelistView->show();
//    textureTabLayout->addWidget(texturelistView);

    texturelistView->resizeColumnToContents(2);


    QMdiArea * area = new QMdiArea;
    textureTabLayout->addWidget(area);

    area->addSubWindow(renderPassSelector);
    area->addSubWindow(texturelistView);

    setSelectedPlane(passModel->index(0, 0,QModelIndex()));
    connect(passListView, SIGNAL(clicked(QModelIndex)), this, SLOT(setSelectedPlane(QModelIndex)));
    connect(texturelistView, SIGNAL(clicked(QModelIndex)), this, SLOT(setSelectedTexture(QModelIndex)));
    connect(renderPassSelector, SIGNAL(currentIndexChanged(QString)), this, SLOT(changePlaneSource(QString)));
    connect(renderPassSelector, SIGNAL(currentIndexChanged(QString)), passListView, SLOT(updateGeometries()));
}

int main(int argc, char *argv[]) {
    Editor(argc, argv).run();
}

