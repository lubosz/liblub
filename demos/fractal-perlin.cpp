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
#include "fractal-perlin.h"
#include <string>
#include <QApplication>
#include "System/Application.h"
#include "Load/SceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Scene/Scene.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include <QPainter>
#include <math.h>
#include "Procedural/ProcTextures.h"
#include "Shader/Shaders.h"
#include "Procedural/Geometry.h"
#include "Renderer/RenderPasses.h"

PerlinDemo::PerlinDemo() : Demo("perlin") {
}

PerlinDemo::~PerlinDemo() {}

void PerlinDemo::init() {
    Scene::Instance().addCamera(QVector3D(0.034992, -0.460323, -1.51176),
                                QVector3D(-0.057455, -0.861629, -0.504276));


    //    QImage * image = ProcTextures::makeGlow(QSize(1000,2000),40.0f, 0.1f);
    //
    //    Texture * textTexture = TextureFactory::Instance().load(image,"myTexture");
    //
    //    Material * material = new EmptyMat();
    //
    //    material->shaderProgram = SceneData::Instance().shaderPrograms.value("Texture");
    //    material->addTexture(textTexture);

    QList<string> attributes = QList<string> () << "normal" << "uv";

    perlinNoise = new VertFragProgram("Fractal/perlin",attributes);


    Node * plane = new Node("Plane", QVector3D(0,0,-2), 1, Geometry::plane(attributes, QRectF(-1,-1,2,2)), perlinNoise);
    //    plane->transparent = true;
    plane->setRotation(QVector3D(-90,0,180));
    SceneGraph::Instance().addNode(plane);
}
void PerlinDemo::draw(){
    perlinNoise->use();
    perlinNoise->setUniform("time", Timer::getTime());
    OnePass::draw();
}
