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
#include "System/Application.h"
#include "Mesh/MengerSponge.h"

class Sponge : public Application {
 public:
  Sponge() {
    programName = "Sponge";
  }

  void scene() {
    SceneGraph::Instance().light =
        new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));

      Material * material = new UbershaderColor(QVector3D(1, 1, 1));

      for (int i = 0; i < 5; i++) {
        MengerSponge * sponge = new MengerSponge(i);
      Node * node = new Node(
          "sponge",
          {static_cast<float>(i-2.5), static_cast<float>(i*3-6), -5},
          1,
          sponge->getMesh(),
          material);
      SceneGraph::Instance().addNode(node);
      }

    Node * plane =
        new Node("Plane", {0, -7, 0}, 20,
            MeshFactory::load("plane.obj"), material);
    plane->setReceiveShadows(true);
    plane->setCastShadows(false);
    SceneGraph::Instance().addNode(plane);
  }
};

int main(int argc, char *argv[]) {
  Sponge().run();
}
