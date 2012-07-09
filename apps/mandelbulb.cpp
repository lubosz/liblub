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
#include "Material/Shaders.h"
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

      for(unsigned x = 0; x < resolution; x++) {
          for(unsigned y = 0; y < resolution; y++) {
              for(unsigned z = 0; z < resolution; z++) {

              QVector3D position = QVector3D(x * density, y * density, z * density);

              mesh->vertex("position", position);
              double color = double(x)/double(resolution);
              mesh->vertex("color", QVector3D(color, color, color));
              }
          }
      }

      mesh->makeLinearIndex();

      mesh->init();
      mesh->setDrawType(GL_POINTS);
      return mesh;
  }

  void scene() {
      QList<string> attributes = QList<string>() << "color";
      Mesh* mandelBulb = bulb(attributes, 100, 0.1);

      ShaderProgram * stars = new SimpleProgram("Particle/stars",attributes);

      Node * plane = new Node("Plane", QVector3D(-5,-10,-15), 1, mandelBulb, stars);
      SceneGraph::Instance().addNode(plane);

  }
  void renderFrame() {
      OnePass::draw();
  }
};

int main(int argc, char *argv[]) {
  MandelbulbApp(argc, argv).run();
}

