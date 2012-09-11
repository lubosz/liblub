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

#ifndef PLANETEDITOR_H
#define PLANETEDITOR_H

#define QT_NO_GRAPHICSEFFECT 1

#include <QHBoxLayout>
#include <QComboBox>

#include "Application/Qt/QtApplication.h"
#include "Renderer/RenderPasses.h"
#include "PlanetWidget.h"
#include "planets/planets.h"

class PlanetEditor: public QtApplication {
    Q_OBJECT
public:

    PlanetEditor(int &argc, char **argv);

    ~PlanetEditor();

    PlanetsDemo * demo;

    void scene();
    void renderFrame();

    PlanetWidget * focusPlanet();
    void initWidgets(QSplitter *mainSplitter);

public slots:
    void setExposure(double exposure);
    void setPostprocessing(bool post);

};

#endif
