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
#include <QPainter>


class LoadApp: public Application {
 public:
  RenderSequence * shadowSequence;

  explicit LoadApp(string sceneName) {
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~LoadApp() {}


  void scene() {
//    unsigned uniformCount = 4;
//    GLuint* uniformIndices= new GLuint[uniformCount];
//    const char* uniformStrings[4] = {
//        "position",
//        "diffuseColor",
//        "specularColor",
//        "direction",
//    };
//    printf("Buffer Struct Size %ld\n", sizeof(lightBufferData));
//    lightBuffer->alloc(bufferSize);
//      GLint * bufferOffsets = new GLint[uniformCount];
//
//
//      //first, get indices of current lightsource members:
//      glGetUniformIndices(
//          uberShader->getReference(),
//          uniformCount,
//          uniformStrings,
//          uniformIndices
//      );
//      glError;
//      //second, get offset in buffer for those members, indentified by the queried indices:
//      glGetActiveUniformsiv(
//          uberShader->getReference(),
//          uniformCount,
////          0,
//          uniformIndices,
//          GL_UNIFORM_OFFSET,
//          bufferOffsets
//      );

    sceneLoader->load();
    shadowSequence = new RenderSequence();
#ifdef USE_FBO
    Material * multilightMat = SceneData::Instance().getMaterial("white");
    multilightMat->addTexture(shadowSequence->renderPasses[0]->targetTexture);
#endif
    SceneData::Instance().initLightBuffer("multilight", "LightSourceBuffer");

  }
  void renderFrame(){
    shadowSequence->render();
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  LoadApp("multilight").run();
}

