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
#include "fractal-julia-cpu.h"
#include "Load/SceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Renderer/RenderPasses.h"
#include "System/Application.h"
#include "Shader/Shaders.h"
#include <QVariantAnimation>
#include "Procedural/Geometry.h"
#include "Texture/Textures.h"
#include "System/TemplateEngine.h"
#include "Procedural/FractalMesh.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include <complex>
#include <QVector2D>

using std::complex;

JuliaDemo::JuliaDemo() : Demo("julia") {
    phase = 1;
    attributes = QList<string>() << "color";
}

JuliaDemo::~JuliaDemo() {
    delete node;
}

Mesh * JuliaDemo::plane(const QList<string> & attributes, unsigned resolution, float density) {
    Mesh * mesh = new Mesh(attributes);

    for(unsigned iY=0; iY < resolution; iY++) {
        for(unsigned iX=0; iX < resolution; iX++) {    /* initial value of orbit Z0 */

            QVector3D position = QVector3D(iX * density, iY * density, 0);

            mesh->vertex("position", position);
            //            mesh->vertex("color",1,0,0);
        }
    }

    mesh->calculateNormals(resolution);
    mesh->calculatePlaneIndex(resolution);

    mesh->init();
    mesh->setDrawType(GL_TRIANGLE_STRIP);
    return mesh;
}

Mesh * JuliaDemo::sinePlane(const QList<string> & attributes, unsigned resolution, float density, float phase) {
    Mesh * mesh = new Mesh(attributes);

    int res2 = resolution / 2;
    for(int iY=-res2; iY < res2; iY++) {
        for(int iX=-res2; iX < res2; iX++) {
            float x = iX * density;
            float y = iY * density;
            QVector2D fromCenter = QVector2D(x, y);
            mesh->vertex("position", x,1.0/fromCenter.length() * 10.0 * cos(sqrt(x*x+y*y) + phase), y );

            mesh->vertex("uv", static_cast<float>(iX)/res2, static_cast<float>(iY)/res2);
            //            mesh->vertex("color",1,0,0);
        }
    }

    mesh->calculatePlaneIndex(resolution);
    mesh->calculateNormals(resolution);

    mesh->init();
    mesh->setDrawType(GL_TRIANGLE_STRIP);
    return mesh;
}

void JuliaDemo::init() {
    ShaderProgram * ocean = new VertFragProgram("Color/vertexcolor", attributes);

    //    mesh = sinePlane(attributes, 100 , 1, phase);
    fractal = new FractalMesh(attributes, 1000 , .01, 2);
    //fractal = new FractalMesh(attributes, 400 , .01, 0.5);
    mesh = plane(attributes, 10, 1);

    fractal->setRange(3.0);

    node = new Node("Test", QVector3D(-5,-5,-5), 1, fractal, ocean);
    SceneGraph::Instance().addNode(node);

    Camera * cam = Scene::Instance().getCurrentCamera();
    cam->setDirection(QVector3D(-1,-5,-1));
    cam->update();
}

void JuliaDemo::draw() {
    fractal->regenerate();
    phase -= 0.001;
    fractal->C = complex<double>(phase,0.156);
    OnePass::draw();
}

