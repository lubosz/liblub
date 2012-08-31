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
#include "tesselation.h"
#include "Renderer/OpenGL.h"
#include <QApplication>
#include "System/Application.h"
#include "System/Logger.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Material/Textures.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Scene/Scene.h"

TesselationApp::TesselationApp() : Demo("tesselation") {
    shader = new ShaderProgram("Tesselation");
}

TesselationApp::~TesselationApp() {}

void TesselationApp::init() {
    Light * light;

    QList<string> attributes;

    shader->attachShader("Tesselation/Tesselation.vert",GL_VERTEX_SHADER);
    shader->attachShader("Tesselation/Tesselation.eval",GL_TESS_EVALUATION_SHADER);
    shader->attachShader("Tesselation/Tesselation.cont",GL_TESS_CONTROL_SHADER);
    shader->attachShader("Tesselation/Tesselation.geom",GL_GEOMETRY_SHADER);
    shader->attachShader("Tesselation/Tesselation.frag",GL_FRAGMENT_SHADER);
    shader->init(attributes);

    shader->setUniform("TessLevelInner",1.0f);
    shader->setUniform("TessLevelOuter",1.0f);
    shader->setUniform("LightPosition", QVector3D(0.25, 0.25, 1));

    camera = Scene::Instance().getCurrentCamera();
    camera->setPosition(QVector3D(0, 0, 25));
    camera->update();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    Scene::Instance().addLight("foolight", light);

    Mesh * mesh = MeshLoader::load(attributes, "earth.obj");
    //    Mesh * mesh = Geometry::gluSphere(10.0f, 100, 50);
    //    Mesh * mesh = Geometry::makeIcosahedron();
    mesh->setDrawType(GL_PATCHES);

    groundNode = new Node("ground", QVector3D(0, 0, 0), 10, mesh, shader);
    groundNode->setRotation(QVector3D(90,0,0));
    glError;

    //    fontOverlay->addText("tess", "Tess");
    //    fontOverlay->addText("dist", "Dist");

    Texture * groundTexture = new TextureFile("terrain/mud.jpg","diffuse");
    Texture * noise = new TextureFile("terrain-noise-blur.jpg","noise");
    shader->addTexture(groundTexture);
    shader->addTexture(noise);
    OpenGL::Instance().setWire(true);
}
void TesselationApp::draw() {
    int maxTess = 30;
    float tessStartDistance = 8;
    float scale = maxTess - (camera->position.length() - tessStartDistance);

    std::stringstream tess;
    tess << "Tess " << int(scale);
    //      fontOverlay->updateText("tess",tess.str());

    std::stringstream dist;
    dist << "Dist " << camera->position.length();
    //      fontOverlay->updateText("dist",dist.str());

    if (scale > 1){
        shader->use();
        shader->setUniform("TessLevelInner",scale);
        shader->setUniform("TessLevelOuter",scale);
    }
    OpenGL::Instance().clear();
    glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_CULL_FACE);
    groundNode->setView(camera);
    glError;
#ifdef USE_OPENGL3
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    groundNode->draw();
#endif
    glError;
}


