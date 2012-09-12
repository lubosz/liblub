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

DemoLauncher::DemoLauncher() {
    addDemos();
    currentDemo = nullptr;
}

DemoLauncher::DemoLauncher(int &argc, char **argv) : DemoLauncher() {
    if (argc != 2) {
        LogError << "No Scene Specified. Try: ./load name";
        listDemos();
    } else {
        chooseDemo(argv[1]);
    }

    if (currentDemo == nullptr)
        exit(0);
}

DemoLauncher::DemoLauncher(string demo) : DemoLauncher() {
    chooseDemo(demo);
    if (currentDemo == nullptr)
        exit(0);
}

DemoLauncher::~DemoLauncher() {
    delete currentDemo;
}

void DemoLauncher::chooseDemo(const string& demo) {
    QString arg = QString::fromStdString(demo);

    if (arg == "-l") {
        std::cout << demoList() << "\n";
        exit(0);
    }
    if (demos.contains(demo)) {
        currentDemo = demos[demo];
    } else if (arg.contains(".xml")){
        currentDemo = new LoadXml("xml scene", demo);
    } else {
        currentDemo = new LoadAssimp("assimp scene", demo);
//                LogError << "No such demo" << arg;
//                listDemos();
    }
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
