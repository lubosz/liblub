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
#include "System/GUI.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "Material/UniformBuffer.h"
#include <QPainter>

class LoadApp: public Application {
 public:


  explicit LoadApp(string sceneName) {
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~LoadApp() {}


  void scene() {
    string bufferName = "LightSourceBuffer";
    unsigned uniformCount = 4;
    GLuint* currentUniformIndices= new GLuint[uniformCount];
    const char* uniformStrings[4] = {
        "position",
        "diffuseColor",
        "specularColor",
        "direction",
    };

    GLint bufferSize;

    sceneLoader->load();

    UniformBuffer * lightBuffer = new UniformBuffer();
    lightBuffer->bind();
    ShaderProgram * uberShader = SceneData::Instance().getProgram("multilight");
    uberShader->bindUniformBuffer("LightSourceBuffer",0,lightBuffer->getHandle());

    GLuint uniBlockIndex = glGetUniformBlockIndex(uberShader->getReference(), bufferName.c_str());
    glGetActiveUniformBlockiv(
      uberShader->getReference(),
      uniBlockIndex,
      GL_UNIFORM_BLOCK_DATA_SIZE,
      &bufferSize
    );

    lightBuffer->alloc(bufferSize);

    foreach(Light* light, SceneData::Instance().lights){

      GLint * bufferOffsets = new GLint[uniformCount];

      printf("Found Light %s\n", SceneData::Instance().lights.key(light).c_str());

      //first, get indices of current lightsource members:
      glGetUniformIndices(
          uberShader->getReference(),
          uniformCount,
          uniformStrings,
          currentUniformIndices
      );
      glError;
      //second, get offset in buffer for those members, indentified by the queried indices:
      glGetActiveUniformsiv(
          uberShader->getReference(),
          //uniformCount,
          0,
          currentUniformIndices,
          GL_UNIFORM_OFFSET,
          bufferOffsets
      );
      glError;
    }

  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  LoadApp("multilight").run();
}

