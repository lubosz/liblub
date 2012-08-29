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

#include "planet.h"
#include <vector>
#include <QTimer>

using std::vector;

class GravityApp: public Application {
	Q_OBJECT
 public:

    vector<Planet*> planets;
    QTimer *simulationTimer;

  GravityApp(int argc, char *argv[]);
  ~GravityApp();

  void scene();
  void renderFrame();

public slots:
  void simulatePlanets();
};

