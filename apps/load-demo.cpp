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
#include "gravity/gravity.h"

class LoadApp: public Application {
public:

    Demo * currentDemo;
    QMap<string, Demo*> demos;

    LoadApp(int &argc, char **argv) : Application(argc,argv) {
        addDemos();

        currentDemo = nullptr;

        if (argc != 2) {
            LogError << "NO SCENE SPECIFIED. Try; ./load-demo name";
            listDemos();
        } else {

            string arg = argv[1];
            if (arg == "-l") {
                std::cout << demoList() << "\n";
                exit(0);
            }
            if (demos.contains(arg)) {
                currentDemo = demos[arg];
            } else {
                LogError << "No such demo" << arg;
                listDemos();
            }
        }

        if (currentDemo == nullptr)
            exit(0);
    }

    void listDemos() {
        LogInfo << "Possible Demos:" << demoList();

    }

    string demoList() {
        string allDemos;
        foreach (Demo * demo, demos)
            allDemos += demo->name + " ";
        return allDemos;
    }

    void addDemos() {
        addDemo(new Sponge());
        addDemo(new AnimationDemo());
        addDemo(new JuliaDemo());
        addDemo(new PerlinDemo());
        addDemo(new DepthOfFieldDemo());
        addDemo(new MandelbulbDemo());
        addDemo(new PulseDemo());
        addDemo(new RaycastingDemo());
        addDemo(new RecursiveSpongeApp());
        addDemo(new SSSApp());
        addDemo(new TesselationApp());
        addDemo(new GravityDemo());
    }

    void addDemo(Demo * demo) {
        demos.insert(demo->name, demo);
    }

    ~LoadApp() {
        delete currentDemo;
    }

    void scene() {
        currentDemo->init();
    }
    void renderFrame(){
        currentDemo->draw();
    }
};

int main(int argc, char **argv) {
    LoadApp(argc,argv).run();
}
