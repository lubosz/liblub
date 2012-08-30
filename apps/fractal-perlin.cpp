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
#include <string>
#include <QApplication>
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Scene/Scene.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include <QPainter>
#include <math.h>
#include "Material/ProcTextures.h"
#include "Material/Shaders.h"
#include "Mesh/Geometry.h"
#include "Renderer/RenderPasses.h"


class LoadApp: public Application {
 public:

   ShaderProgram * perlinNoise;

  explicit LoadApp(int argc, char *argv[]) : Application(argc,argv) {
  }

  ~LoadApp() {}

  void scene() {
//    QImage * image = ProcTextures::makeGlow(QSize(1000,2000),40.0f, 0.1f);
//
//    Texture * textTexture = TextureFactory::Instance().load(image,"myTexture");
//
//    Material * material = new EmptyMat();
//
//    material->shaderProgram = SceneData::Instance().shaderPrograms.value("Texture");
//    material->addTexture(textTexture);

    QList<string> attributes;
    attributes.push_back("uv");
//    attributes.push_back("normal");

    perlinNoise = new VertFragProgram("Noise/perlin",attributes);


    Node * plane = new Node("Plane", QVector3D(0,0,-2), 1, Geometry::plane(attributes, QRectF(-1,-1,2,2)), perlinNoise);
//    plane->transparent = true;
    plane->setRotation(QVector3D(-90,0,180));
    SceneGraph::Instance().addNode(plane);
  }
  void renderFrame(){
    perlinNoise->use();
    perlinNoise->setUniform("time", Timer::getTime());
    OnePass::draw();
  }
};

int main(int argc, char *argv[]) {
  LoadApp(argc,argv).run();
}

