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
#include "Scene/SceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Renderer/RenderSequence.h"
#include "System/Application.h"

class LoadApp: public Application {
 public:

  SceneLoader *sceneLoader;
  RenderSequence * shadowSequence;

  LoadApp(int argc, char *argv[]) : Application(argc,argv) {
    sceneLoader = new SceneLoader("nice.xml");
  }

  ~LoadApp() {}

  void scene() {
    sceneLoader->load();
    shadowSequence = new RenderSequence();
  }
  void renderFrame(){
    shadowSequence->render();
    Node * sponge = SceneGraph::Instance().getNode("menger sponge");
    sponge->rotation += QVector3D(0.1,0.2,0);
    sponge->update();
  }
  void initWidgets(QHBoxLayout * mainLayout) {}
};

int main(int argc, char *argv[]) {
  LoadApp(argc, argv).run();
}

