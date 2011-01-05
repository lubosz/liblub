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
#include "Scene/SceneLoader.h"

class LoadApp: public Application {
 public:


  explicit LoadApp(string sceneName) {
    programName ="XML Loader";
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~LoadApp() {}

  void scene() {
    sceneLoader->load();
  }
};

int main(int argc, char *argv[]) {
  if (argc == 2) {
    LoadApp(argv[1]).run();
  } else {
    Logger::Instance().log("NO SCENE SPECIFIED", "Try;", "./load test");
  }
}

