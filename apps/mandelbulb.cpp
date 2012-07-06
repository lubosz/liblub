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
#include "Scene/SceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Renderer/RenderPasses.h"
#include "System/Application.h"
#include <QVariantAnimation>

class MandelbulbApp: public Application {
public:


  MandelbulbApp(int argc, char *argv[]) :
    Application(argc, argv) {
  }

  ~MandelbulbApp() {
  }

  Mesh * bulb(const QList<string> & attributes, unsigned resolution, float density) {
      Mesh * mesh = new Mesh(attributes);

      for(unsigned x=0; iY < resolution; x++) {
          for(unsigned y=0; iX < resolution; y++) {
              for(unsigned z=0; iX < resolution; z++) {

              QVector3D position = QVector3D(x * density, y * density, z * density);

              mesh->vertex("position", position);
              }
          }
      }

//      mesh->calculateNormals(resolution);
      mesh->makeLinearIndex();
//      mesh->calculatePlaneIndex(resolution);

      mesh->init();
      mesh->setDrawType(GL_POINTS);
      return mesh;
  }

  void scene() {
      QList<string> foo = QList<string>() << "color";
      Mesh* mandelBulb = bulb(foo, 100, 1);

      ShaderProgram * stars = new SimpleProgram("Particle/stars",attributes);

      Node * plane = new Node("Plane", { 0,0,0 }, 1, mandelBulb, stars);
      SceneGraph::Instance().addNode(plane);

  }
  void renderFrame() {
      OnePass::draw();
  }
};

int main(int argc, char *argv[]) {
  MandelbulbApp(argc, argv).run();
}

