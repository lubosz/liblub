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
#include "Scene/Scene.h"
#include "System/Demo.h"

class Ball : public Node {
public:
    Ball();
    QVector3D makeVectorFromPolarCoords(float amount, float length);
};

class PulseDemo: public Demo {
public:

    float rotation;
    float pulse;
    vector<Ball*> balls;

    PulseDemo();
    ~PulseDemo();

    void init();
    void draw();
};

