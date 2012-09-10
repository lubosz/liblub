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
#include "load-assimp.h"
#include "Renderer/DeferredRenderer.h"
#include "Scene/Scene.h"
#include "Load/AssimpSceneLoader.h"

LoadAssimp::LoadAssimp(string name, string path) : Demo(name) {
    scenePath = path;
}

LoadAssimp::~LoadAssimp() {
}

void LoadAssimp::init() {
    Scene::Instance().getCurrentCamera()->setPosition(QVector3D(-1.43765, 5.130675, -1.20157));
    Scene::Instance().getCurrentCamera()->setDirection(QVector3D(0.741701, -0.0836778, 0.66549));
    Scene::Instance().getCurrentCamera()->update();

    DeferredRenderer::Instance().initSky("cubemaps/sky");

    AssimpSceneLoader::Instance().load(scenePath);
    DeferredRenderer::Instance().init();
}
void LoadAssimp::draw() {
    DeferredRenderer::Instance().draw();
}
