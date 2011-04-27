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
#include "System/GUI.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include <QPainter>
#include <math.h>
#include "Material/ProcTextures.h"
#include "Mesh/Geometry.h"

class LoadApp: public Application {
 public:

   RenderPass * defaultPass;
   ShaderProgram * perlinNoise;

  explicit LoadApp(string sceneName) {
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~LoadApp() {}



  void scene() {
    sceneLoader->load();
    defaultPass = new LightTogglePass();
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

//    Material * material = new Simple("Noise/perlin",attributes);
    Material * zoomIn = new Simple("Texture/texture",attributes);
    Material * zoomOut = new Simple("Texture/texture",attributes);
    Texture * groundTexture = TextureFactory::Instance().load("terrain/mud.jpg","diffuse");
    zoomIn->addTexture(groundTexture);
    zoomOut->addTexture(groundTexture);
//    perlinNoise = material->getShaderProgram();


    Node * plane = new Node("Plane", { 10,8,-3 }, 10, Geometry::plane(attributes, QRectF(-1,-1,2,2)), zoomIn);
    Node * plane2 = new Node("Plane", { -10,8,-3 }, 10, Geometry::plane(attributes, QRectF(-1,-1,2,2)), zoomOut);
//    plane->transparent = true;
    plane->setRotation(QVector3D(-90,0,180));
    plane2->setRotation(QVector3D(-90,0,180));
    SceneGraph::Instance().addNode(plane);
    SceneGraph::Instance().addNode(plane2);


    zoomIn->getShaderProgram()->use();
    zoomIn->getShaderProgram()->setUniform("scaleUv", 0.01f);

    zoomOut->getShaderProgram()->use();
    zoomOut->getShaderProgram()->setUniform("scaleUv", 100.0f);

    GUI::Instance().init();
  }
  void renderFrame(){
    defaultPass->render();
//    float time = float(Timer::Instance().secoundsPassed) + float(Timer::Instance().nanosecoundsPassed)/1000000000.0;
//    perlinNoise->use();
//    perlinNoise->setUniform("time", time);
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  LoadApp("proctex").run();
}

