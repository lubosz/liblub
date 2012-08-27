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
#include "Scene/Scene.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include <QPainter>
#include <math.h>
#include "Material/ProcTextures.h"
#include "Material/Textures.h"
#include "Mesh/Geometry.h"
#include "Renderer/OpenGL.h"
#include "Renderer/FrameBuffer.h"
#include "Scene/SceneLoader.h"
#include "Material/Shaders.h"
#include "Scene/Scene.h"
#include "Renderer/RenderPasses.h"
#include "Scene/SceneGraph.h"

class LoadApp: public Application {
 public:

    Texture * groundTexture;
    Node * plane;
    Node * plane2;

  explicit LoadApp(int argc, char *argv[]) : Application(argc,argv) {
  }

  ~LoadApp() {
        delete groundTexture;
        delete plane;
        delete plane2;
    }

  void scene() {
    QList<string> attributes;
    attributes.push_back("uv");

    ShaderProgram * zoomIn;
    ShaderProgram * zoomOut;
    zoomIn = new SimpleProgram("Texture/texture",attributes);
    zoomOut = new SimpleProgram("Texture/texture",attributes);
    groundTexture = new TextureFile("terrain/mud.jpg","diffuse");
    zoomIn->addTexture(groundTexture);
    zoomOut->addTexture(groundTexture);

    plane = new Node("Plane", QVector3D(10,8,-3), 10, Geometry::plane(attributes, QRectF(-1,-1,2,2)), zoomIn);
    plane2 = new Node("Plane", QVector3D(-10,8,-3), 10, Geometry::plane(attributes, QRectF(-1,-1,2,2)), zoomOut);
    plane->setRotation(QVector3D(-90,0,180));
    plane2->setRotation(QVector3D(-90,0,180));
    SceneGraph::Instance().addNode(plane);
    SceneGraph::Instance().addNode(plane2);

    zoomIn->use();
    zoomIn->setUniform("scaleUv", 0.01f);

    zoomOut->use();
    zoomOut->setUniform("scaleUv", 100.0f);
  }
  void renderFrame(){
      OnePass::draw();
  }
};

int main(int argc, char *argv[]) {
  LoadApp(argc,argv).run();
  return 0;
}

