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
#include "System/Application.h"
#include "Scene/Scene.h"
#include "System/Logger.h"
#include "Scene/TreeSponge.h"
#include "Mesh/Geometry.h"
#include "Material/Textures.h"
#include "Renderer/OpenGL.h"
#include "Renderer/FrameBuffer.h"
#include "Material/Shaders.h"
#include "Scene/Scene.h"
#include "Scene/InstancedSponge.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderPasses.h"
#include "Mesh/MengerSponge.h"
#include "Scene/SceneGraph.h"

class RomanescoNode : public Node {
public:



    RomanescoNode() {
        position = QVector3D(0,0,0);
        QList<string> attributes = QList<string>() << "uv";




    }

    QVector3D makeVectorFromPolarCoords(float amount, float length) {
        QVector3D returnMe = QVector3D(length, -length/10.0, 0);
        QMatrix4x4 rotation = QMatrix4x4();
        rotation.setToIdentity();
        rotation.rotate(amount, QVector3D(0, 1, 0));

        return rotation * returnMe;
    }

    ~RomanescoNode() {

    }
};

class RomanescoApp: public Application {
 public:

    float rotation;
    Texture * texture;
    Mesh* mesh;

    float foo;
    vector<RomanescoNode*> romanescos;


  RomanescoApp(int argc, char *argv[]) : Application(argc,argv) {
      foo = 0;
      rotation = 0;
  }

  ~RomanescoApp() {
//      delete texture;
      for(auto romanesco : romanescos)
        delete romanesco;
  }

  void scene() {

      QList<string> attributes = QList<string>() << "uv";
      ShaderProgram * shader = new TemplateProgram("instancing",attributes);
  //    sponge->initBuffers(material);

      texture = new TextureFile("diamond.png", "diffuse");
      texture->bind();
      texture->filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
      shader->addTexture(texture);
//      mesh = Geometry::tetrahedron(attributes);
      mesh = Geometry::sphere(attributes, 1, 20, 20);

      for (int i = 0; i < 500; i++) {
          RomanescoNode * romanesco;
        romanesco = new RomanescoNode();
        romanesco->setShader(shader);
        romanesco->setMesh(mesh);
//        rotation += .1;
//        QVector3D theVector = romanesco->makeVectorFromPolarCoords(10*rotation, rotation/10);
//        romanesco->setSize(rotation/100);
//        romanesco->setDirection(theVector);
//        romanesco->setPosition(theVector);
//        romanesco->update();
        SceneGraph::Instance().addNode(romanesco);
        romanescos.push_back(romanesco);

      }
  }

  void renderFrame(){
      foo += .1;
        rotation = 0;
      for (auto romanesco : romanescos) {
          QVector3D theVector = romanesco->makeVectorFromPolarCoords(-10*rotation, rotation/10);
          romanesco->setSize((sin(foo+rotation)+1.0)/10.0+rotation/100);
          romanesco->setDirection(theVector);
          romanesco->setPosition(theVector);
          romanesco->update();
          rotation += .1;

      }
    OnePass::draw();
  }

};

int main(int argc, char *argv[]) {
  RomanescoApp app(argc,argv);
  app.run();
}


