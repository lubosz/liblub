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
#include "sponge.h"
#include "Mesh/MengerSponge.h"
#include "Scene/SceneData.h"
#include "Mesh/Geometry.h"

Sponge::Sponge(int argc, char *argv[]) :
  Application(argc, argv) {
  SceneData::Instance().name = "Sponge";
}

void Sponge::scene() {
//  window->setWindowTitle("Sponge");
  shadowSequence = new RenderSequence();
  Material * material = new EmptyMat();
  material->init();
  material->addTexture(shadowSequence->renderPasses[0]->targetTexture);
  vector < string > flags = {
    "receiveShadows",
    "useSpotLight",
    "usePCF"
  };
  material->shaderProgram->attachVertFrag("Color/PhongColor", flags);
  QList<string> attributes;
  attributes.push_back("uv");
  attributes.push_back("normal");
  attributes.push_back("tangent");
  material->done(attributes);
  material->shaderProgram->setUniform("ambientSceneColor",QVector4D(0.1, 0.1,0.1, 1.0));
  material->shaderProgram->setUniform("diffuseMaterialColor",QVector4D(1,1,1,1));
  material->shaderProgram->setUniform("specularMaterialColor",
            QVector4D(0.8, 0.8, 0.8, 1.0));
  material->shaderProgram->setUniform("shininess",4.3f);
  material->shaderProgram->setUniform("yPixelOffset",1.0f/1200);
  material->shaderProgram->setUniform("xPixelOffset",1.0f/1920);

    for (int i = 0; i < 5; i++) {
      MengerSponge * sponge = new MengerSponge(attributes, i);
      Node * node = new Node("sponge", { static_cast<float> (i - 2.5),
          static_cast<float> (i * 3 - 6), -5 }, 1, sponge->getMesh(), material);
      SceneGraph::Instance().addNode(node);
    }

    Node * plane = new Node("Plane", { 0, -30, 0 }, 20, Geometry::plane(attributes, QRectF(-1,-1,2,2)), material);
    plane->setRotation(QVector3D(90,0,0));
    plane->setReceiveShadows(true);
    plane->setCastShadows(false);
    SceneGraph::Instance().addNode(plane);

    SceneData::Instance().addLight("foolight", new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0)));
}

void Sponge::renderFrame() {
  shadowSequence->render();
}

void Sponge::initWidgets(QHBoxLayout * mainLayout) {
  LogDebug << mainLayout;
}

int main(int argc, char *argv[]) {
  Sponge(argc, argv).run();
}
