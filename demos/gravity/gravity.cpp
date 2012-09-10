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
#include "gravity.h"

#include "Renderer/RenderPasses.h"
#include "Scene/Scene.h"

GravityDemo::GravityDemo() : Demo("gravity") {
}

GravityDemo::~GravityDemo() {
}

void GravityDemo::init() {
    float sunDistance = 1.496e+8;
    float moonDistance = 356400;

    sunDistance /= 4.0;
    moonDistance /= 4.0;

    SimPlanet * moon = new SimPlanet("Earth/MoonMap_2500x1250.jpg", QVector3D(-sunDistance,0,moonDistance), 1737.1);
    moon->mass = 7.3477e+22;
    QVector3D moonDir = QVector3D(moonDistance - sunDistance, 0, 0) - moon->position;
    //    moon->velocity = 1022 * moonDir.normalized();
    moon->velocity = 30 * moonDir.normalized();
    planets.push_back(moon);

    SimPlanet * earth = new SimPlanet("earthmap1k.jpg", QVector3D(-sunDistance,0,0), 6371);
    earth->mass = 5.9736e+24;

    QVector3D earthDir = QVector3D(0, 0, -sunDistance) - earth->position;
    //    moon->velocity = 1022 * moonDir.normalized();
    earth->velocity = 3 * earthDir.normalized();

    planets.push_back(earth);

    SimPlanet * sun = new SimPlanet("Planets/sun.jpg", QVector3D(0,0,0), 6.96342e+5);
    sun->mass = 1.9891e+30;
    planets.push_back(sun);

    foreach (SimPlanet * planet, planets)
        planet->init();

    Scene::Instance().getCurrentCamera()->farClip = 100000.0;
    Scene::Instance().getCurrentCamera()->updatePerspective();

    simulationTimer = new QTimer();
    connect(simulationTimer, SIGNAL(timeout()), this, SLOT(simulatePlanets()));
    simulationTimer->start(0);

    Scene::Instance().getCurrentCamera()->setPosition(planets[1]->node->position + QVector3D(0,5,5));
    Scene::Instance().getCurrentCamera()->update();
}

void GravityDemo::simulatePlanets() {
    foreach (SimPlanet * planet, planets)
        foreach (SimPlanet * planet2, planets)
            if (planet != planet2)
                planet->influenceByPlanet(planet2);

    foreach (SimPlanet * planet, planets)
        planet->move();
}

void GravityDemo::draw(){
    OnePass::draw();
}
