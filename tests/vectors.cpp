#include <cppunit/TestCase.h>
 #include <cppunit/TestListener.h>
 #include <cppunit/Test.h>
 #include <time.h>    // for clock()

#include "System/Logger.h"
#include "System/Config.h"
#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Scene/Light.h"
#include "Material/Materials.h"
#include "Mesh/Mesh.h"
#include "Mesh/MengerSponge.h"

class Clock {
  clock_t starttime;
public:
  void start(){
    starttime = clock();
  }
  void end(){
    clock_t end = clock();
    double cps = CLOCKS_PER_SEC;
    double total = (end-starttime);
    double time = total / cps;
    printf("%lf secounds\n", time);
  }
};

class VectorTest: public CppUnit::TestCase {
  Clock _chronometer;
public:
  VectorTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    _chronometer.start();
    for(int i = 0; i < 1000000; i++){
      vector<string> cpp = { "foo", "bar" };
      vector<string> bpp = cpp;
    }
    _chronometer.end();
//
//    MengerSponge * sponge = new MengerSponge(3);
//    Camera * camera = new Camera();
//
//    Light * light = new Light(QVector3D(0, 0, 0),QVector3D(0, -5, 0));
//
//    Material * material = new EmptyMat();
//    Mesh * mesh = new Mesh();
//    Node * node = new Node("", QVector3D(0,0,0), 1, mesh, material);

    //    CPPUNIT_ASSERT( 0 == 0 );

  }
};

class ArrayTest: public CppUnit::TestCase {
  Clock _chronometer;
public:
  ArrayTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    _chronometer.start();
    for(int i = 0; i < 1000000; i++){
      string c[] = { "foo", "bar" };
      string b[2] = c;
    }
    _chronometer.end();
  }
};

int main(int argc, char *argv[]) {
  VectorTest foo("bar");
  foo.runTest();
  ArrayTest foo2("bar");
  foo2.runTest();
}




