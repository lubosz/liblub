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
#define BUFFERSIZE 2000

#include <string>
#include <QApplication>
#include "System/Application.h"
#include "System/GUI.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "Scene/Node.h"
#include "Mesh/Geometry.h"
#include "Material/Textures.h"

class LoadApp: public Application {
 public:
  Material * material;
  Node * node;
  Mesh * cube;
  UniformBuffer * positionBuffer;
  GLint positionBufferSize;

  vector<QVector4D> positionBufferData;


  explicit LoadApp() {
  }

  ~LoadApp() {}

  void scene() {
    QList<string> attributes;
//    attributes.push_back("color");
//    attributes.push_back("normal");
    attributes.push_back("uv");
//    attributes.push_back("tangent");

    material = new Simple("Stuff/instancing",attributes);
    cube = Geometry::cube(attributes);
//    cube = MeshLoader::load(attributes, "monkey.obj");
    cube->setDrawType(GL_TRIANGLES);
    node = new Node("cube", {0,0,-5}, 1, cube, material);


    Texture * texture = new TextureFile("diamond.png", "diffuse");
    texture->bind();
    texture->filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
    material->addTexture(texture);

    initPositionBuffer();

//    for (int x = 0; x < 20; x++){
//      for (int y = 0; y < 20; y++){
//        for (int z = 0; z < 10; z++){
//      SceneGraph::Instance().addNode(new Node("", QVector3D(3*x, 3*y, 3*z), 1, cube, material));
//        }
//      }
//    }
    /*
    SceneData::Instance().addLight("foolight", new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0)));


    for (int x = 0; x < 63; x++){
      for (int y = 0; y < 63; y++){
        SceneGraph::Instance().addNode(new Node("", QVector3D(2*x, 2*((x+y)%5), 2*y), 1, cube, material));
      }
    }
*/

  }
  void renderFrame(){
    RenderEngine::Instance().clear();
    node->setView(SceneData::Instance().getCurrentCamera());
    material->activate();
    cube->draw(4000);
//    SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
  }


  void initPositionBuffer() {
    positionBuffer = new UniformBuffer();
    positionBuffer->bind();

    GLuint uniBlockIndex = glGetUniformBlockIndex(material->getShaderProgram()->getReference(), "positions");
    glGetActiveUniformBlockiv(
      material->getShaderProgram()->getReference(),
      uniBlockIndex,
      GL_UNIFORM_BLOCK_DATA_SIZE,
      &positionBufferSize
    );

    printf("Position Uniform Buffer Size %d\n", positionBufferSize);

    unsigned i = 0;
    for (int x = 0; x < 63; x++){
      for (int y = 0; y < 63; y++){
//        for (int z = 0; z < 10; z++){
//          positionBufferData[i] = QVector3D(2*x, 2*y, 2*z);
//          positionBufferData.push_back(QVector4D(3*x, 3*y, 3*z,1));
        positionBufferData.push_back(QVector4D(2*x, 2*((x+y)%5), 2*y,1));
//          positionBufferData[i].diffuse = QVector4D();
          i++;
//        }
      }
    }


    printf("Position Array Size %ld\n", positionBufferData.size());

//    for (int i = 0; i < 2000; i++) {
//      positionBufferData[i].position = QVector4D(2*i, 0, -5, 1);
//      positionBufferData[i].diffuse = QVector4D();
//    }
    positionBuffer->write(positionBufferData.data(), positionBufferSize);
    material->getShaderProgram()->bindUniformBuffer("positions",0,positionBuffer->getHandle());
    glError;
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  LoadApp().run();
}

