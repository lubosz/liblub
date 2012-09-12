#include "DemoLauncher.h"

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
#include "load-assimp.h"
#include "load-xml.h"
#include "planets/planets.h"

DemoLauncher::DemoLauncher(int &argc, char **argv) {
    addDemos();

    currentDemo = nullptr;

    if (argc != 2) {
        LogError << "No Scene Specified. Try: ./load name";
        listDemos();
    } else {

        string arg = argv[1];
        QString qarg = QString::fromStdString(arg);

        if (arg == "-l") {
            std::cout << demoList() << "\n";
            exit(0);
        }
        if (demos.contains(arg)) {
            currentDemo = demos[arg];
        } else if (qarg.contains(".xml")){
            currentDemo = new LoadXml("xml scene", arg);
        } else {
            currentDemo = new LoadAssimp("assimp scene", arg);
//                LogError << "No such demo" << arg;
//                listDemos();
        }
    }

    if (currentDemo == nullptr)
        exit(0);
}

DemoLauncher::~DemoLauncher() {
    delete currentDemo;
}

void DemoLauncher::listDemos() {
    LogInfo << "Possible Demos:" << demoList();

}

string DemoLauncher::demoList() {
    string allDemos;
    foreach (Demo * demo, demos)
        allDemos += demo->name + " ";
    return allDemos;
}

// All Demos need to be Initialized here
void DemoLauncher::addDemos() {
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
    addDemo(new PlanetsDemo());
}

void DemoLauncher::addDemo(Demo * demo) {
    demos.insert(demo->name, demo);
}

void DemoLauncher::init() {
    currentDemo->init();
}
void DemoLauncher::draw(){
    currentDemo->draw();
}
