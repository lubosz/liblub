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
#include <QApplication>
#include "System/Application.h"
#include "Mesh/MengerSponge.h"
#include "Scene/SceneData.h"

class Sponge : public Application {
 public:
  RenderSequence * shadowSequence;
  Sponge() {
    SceneData::Instance().name = "Sponge";
  }

  void scene() {
    shadowSequence = new RenderSequence();

    Material * material = new EmptyMat();
    material->init();
#ifdef USE_FBO
    material->addTexture(shadowSequence->renderPasses[0]->targetTexture);
#endif
    vector<string> flags = {
        "receiveShadows",
        "useSpotLight",
        "usePCF"
    };
    material->shaderProgram->attachVertFrag("Color/PhongColor", flags);
    material->done();
    material->shaderProgram->setUniform(QVector4D(0.1, 0.1, 0.1, 1.0), "ambientSceneColor");
    material->shaderProgram->setUniform(QVector4D(1,1,1,1), "diffuseMaterialColor");
    material->shaderProgram->setUniform(
            QVector4D(0.8, 0.8, 0.8, 1.0), "specularMaterialColor");
    material->shaderProgram->setUniform(4.3, "shininess");
    material->shaderProgram->setUniform(1.0/1200, "yPixelOffset");
    material->shaderProgram->setUniform(1.0/1920, "xPixelOffset");

    for (int i = 0; i < 5; i++) {
      MengerSponge * sponge = new MengerSponge(i);
      Node * node = new Node("sponge", { static_cast<float> (i - 2.5),
          static_cast<float> (i * 3 - 6), -5 }, 1, sponge->getMesh(), material);
      SceneGraph::Instance().addNode(node);
    }

    Node * plane = new Node("Plane", { 0, -30, 0 }, 20, MeshFactory::plane(), material);
    plane->setRotation(QVector3D(90,0,0));
    plane->setReceiveShadows(true);
    plane->setCastShadows(false);
    SceneGraph::Instance().addNode(plane);

    SceneData::Instance().addLight("foolight", new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0)));
  }
  void renderFrame(){
    shadowSequence->render();
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  Sponge().run();
}
