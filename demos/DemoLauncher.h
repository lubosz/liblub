#ifndef DEMOLAUNCHER_H
#define DEMOLAUNCHER_H

#include <string>
#include <QMap>
#include "System/Demo.h"

using std::string;

class DemoLauncher {
public:
    DemoLauncher(int &argc, char **argv);
    ~DemoLauncher();

    Demo * currentDemo;
    QMap<string, Demo*> demos;

    void listDemos();
    string demoList();
    void addDemos() ;
    void addDemo(Demo * demo);

    void init();
    void draw();

};

#endif // DEMOLAUNCHER_H
