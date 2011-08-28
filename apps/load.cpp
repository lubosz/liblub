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

class LoadApp: public Application {
 public:
  SceneLoader *sceneLoader;

  RenderSequence * shadowSequence;
  explicit LoadApp(string sceneName) {
    fontOverlay = true;
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~LoadApp() {}

  void scene() {
    sceneLoader->load();
    shadowSequence = new RenderSequence();
  }
  void renderFrame(){
    shadowSequence->render();
  }
};
#ifdef LIBLUB_WINDOWS
#include <windows.h>
int WINAPI WinMain(HINSTANCE inst,HINSTANCE prev,LPSTR cmd,int show) {
  LogDebug << "WINMAIN STUFF" << inst << prev << cmd << show;
  QApplication app();
  LoadApp("nice").run();
  return 0;
}
#else
int main(int argc, char *argv[]) {
  if (argc == 2) {
    LoadApp(argv[1]).run();
  } else {
    LogError << "NO SCENE SPECIFIED. Try; ./load test";
  }
  return 0;
}
#endif

