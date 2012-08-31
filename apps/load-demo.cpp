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

#include "sponge.h"
#include "animation.h"
#include "fractal-julia-cpu.h"
#include "mandelbulb.h"
#include "raycasting.h"
#include "tesselation.h"
#include "dof.h"
#include "fractal-perlin.h"
#include "pulse.h"
#include "recursive-sponge.h"
#include "sss.h"

class Sponge;

class LoadApp: public Application {
public:

    Demo * demo;

    LoadApp(int &argc, char **argv) : Application(argc,argv) {
        string name = argv[1];
        if (name == "Sponge")
            demo = new Sponge();
        else if (name == "AnimationDemo")
            demo = new AnimationDemo();
        else if (name == "JuliaDemo")
            demo = new JuliaDemo();
        else if (name == "PerlinDemo")
            demo = new PerlinDemo();
        else if (name == "DepthOfFieldDemo")
            demo = new DepthOfFieldDemo();
        else if (name == "MandelbulbDemo")
            demo = new MandelbulbDemo();
        else if (name == "PulseDemo")
            demo = new PulseDemo();
        else if (name == "RaycastingDemo")
            demo = new RaycastingDemo();
        else if (name == "RecursiveSpongeApp")
            demo = new RecursiveSpongeApp();
        else if (name == "SSSApp")
            demo = new SSSApp();
        else if (name == "TesselationApp")
            demo = new TesselationApp();
        else
            LogFatal << "No Such Demo";

    }

    ~LoadApp() {
        //    delete demo;
    }

    void scene() {
        demo->init();
    }
    void renderFrame(){
        demo->draw();
    }
};

int main(int argc, char **argv) {
      if (argc != 2)
        LogError << "NO SCENE SPECIFIED. Try; ./load-demo name";
      else
    LoadApp(argc,argv).run();
}
