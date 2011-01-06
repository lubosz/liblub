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
#include <vector>
#include "System/Application.h"
#include "Mesh/MengerSponge.h"

class Example : public Application {
 public:
  Example() {
    programName = "Example";
  }

  void scene() {
    Light * light =
        new Light(QVector3D(0.3, 6.6, -3.3), QVector3D(1, -5, -1));
    SceneGraph::Instance().light = light;

    vector<Material*> textureMats = {
        new EarthMaterial(),
        new CubeMap("cubemaps/sky"),
        new Simple("Color/normalcolor"),
        new BrickMaterial(),
        new ProcBumpMaterial(),
        new OceanMat(),
        new NormalMapMaterial(
            "bump/brickwork-texture.jpg",
            "bump/brickwork_normal-map.jpg"),
        new PhongColorMat(QVector3D(0, 1, 0)),
        new SphereMap("spectrum.png",
            "spheremaps/spheremap_bar.jpg"),

        new PhongTexMat("Paper_Texture_by_Spiteful_Pie_Stock.jpg"),

        new MandelMat()
    };
    Mesh * mesh = MeshFactory::load("monkey.blend");
    SceneGraph::Instance().meshPlane(mesh, 10, 3, textureMats);
    Material * starMat = new Simple("Particle/stars");
    SceneGraph::Instance().addNode(
        new Node("", {0, -50, -50},
            MeshFactory::stars(100, 1.0f, 0.0f, 1), starMat));

    MengerSponge * sponge = new MengerSponge(3);

    Material * material = new PhongColorMat(QVector3D(.5, 1, 1));
    SceneGraph::Instance().addNode(
        new Node("", QVector3D(10, 0, -10), 3.0, sponge->getMesh(), material));
  }
};

int main(int argc, char *argv[]) {
  Example().run();
}
