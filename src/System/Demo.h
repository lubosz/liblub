#ifndef DEMO_H
#define DEMO_H

#include <string>

using std::string;

class Demo {
public:
    string name;
    Demo(const string& name);

    virtual ~Demo();

    virtual void init() = 0;
    virtual void draw() = 0;
};

#endif // DEMO_H
