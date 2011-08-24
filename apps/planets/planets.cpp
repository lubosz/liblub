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
#ifdef WITH_Qt
#include <string>
#include <QApplication>
#include <QPainter>
#include "System/TemplateEngine.h"
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Material/ProcTextures.h"
#include "Material/Textures.h"
#include "Material/ShaderProgram.h"
#include "Planet.h"
#include "PlanetWindow.h"
#include "glWidget.h"

int main(int argc, char *argv[]) {
#if LIBLUB_WINDOWS
#include <GL/glew.h>
  initWinGL();
#endif
  QApplication app(argc, argv);

  PlanetWindow window;
  window.resize(QSize(1920,1200));
  //  int desktopArea = QApplication::desktop()->width()
  //      * QApplication::desktop()->height();
  //  int widgetArea = window.width() * window.height();
  //  if (((float) widgetArea / (float) desktopArea) < 0.75f)
  window.show();
  //  else
  //    window.showMaximized();

  return app.exec();
}

#else // WITH_QT
int main(int argc, char *argv[]) {
  (void)argc; (void)argv;
  return 1;
}
#endif
