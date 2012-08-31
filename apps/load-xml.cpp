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
#include "Load/SceneLoader.h"
#include "Scene/Scene.h"
#include "System/Logger.h"
#include "Renderer/OpenGL.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/RenderPasses.h"
#include "Load/SceneLoader.h"
#include "Scene/Scene.h"

class LoadApp: public Application {
 public:
  SceneLoader *sceneLoader;

  LoadApp(int &argc, char **argv) : Application(argc,argv) {
    useFontOverlay = true;
    string sceneName = argv[1];
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~LoadApp() {
    delete sceneLoader;
  }

  void scene() {
    sceneLoader->load();
  }
  void renderFrame(){
      OnePass::draw();
  }
};

int main(int argc, char **argv) {
  if (argc != 2)
    LogError << "NO SCENE SPECIFIED. Try; ./load-xml test";
  else
    LoadApp(argc,argv).run();
}
