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
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include <QPainter>
#include "Mesh/Geometry.h"
#include "Material/ProcTextures.h"
#include "Material/ShaderProgram.h"

class AtmosphereApp: public Application {
 public:

  RenderSequence * shadowSequence;
  ShaderProgram *groundFromAtmosphere, *groundFromSpace,
                *skyFromAtmosphere, *skyFromSpace,
                *spaceFromAtmosphere,*spaceFromSpace,
                *HDR;

  explicit AtmosphereApp(string sceneName) {
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~AtmosphereApp() {}

  void scene() {
    sceneLoader->load();

//    Material * foo = new Simple("Atmo/GroundFromAtmosphere");
//    if(foo==NULL) printf("1");
    groundFromAtmosphere = new ShaderProgram();
    groundFromAtmosphere->attachVertFrag("Atmo/GroundFromAtmosphere");
    groundFromAtmosphere->init();

    groundFromSpace = new ShaderProgram();
    groundFromSpace->attachVertFrag("Atmo/GroundFromSpace");
    groundFromSpace->init();

    skyFromAtmosphere = new ShaderProgram();
    skyFromAtmosphere->attachVertFrag("Atmo/SkyFromAtmosphere");
    skyFromAtmosphere->init();

    skyFromSpace = new ShaderProgram();
    skyFromSpace->attachVertFrag("Atmo/SkyFromSpace");
    skyFromSpace->init();

    spaceFromAtmosphere = new ShaderProgram();
    spaceFromAtmosphere->attachVertFrag("Atmo/SpaceFromAtmosphere");
    spaceFromAtmosphere->init();

    spaceFromSpace = new ShaderProgram();
    spaceFromSpace->attachVertFrag("Atmo/SpaceFromSpace");
    spaceFromSpace->init();

//    HDR = new ShaderProgram();
//    HDR->attachShader("Atmo/HDR.vert", GL_VERTEX_SHADER);
//    HDR->attachShader("Atmo/HDRRect.frag", GL_FRAGMENT_SHADER); //HDRSquare
//    HDR->init();

    Mesh * sphere = Geometry::gluSphere(10, 100, 50);

    Texture * earthMap = TextureFactory::Instance().load("earthmap1k.jpg",
        "myTexture");
//    Texture * glow = TextureFactory::Instance().load(ProcTextures::makeGlow(
//        QSize(1000, 2000), 40.0f, 0.1f), "glow");

    Material * material = new Simple("Color/debug");
    material->addTexture(earthMap);

    Node * sphereNode = new Node("sphere", { 0, 0, 0 }, 1, sphere, material);
    sphereNode->setRotation(QVector3D(-90, 0, 180));
    SceneGraph::Instance().addNode(sphereNode);



    shadowSequence = new RenderSequence();
  }
  void renderFrame(){
    shadowSequence->render();
  }
};


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  AtmosphereApp("atmo").run();
}

