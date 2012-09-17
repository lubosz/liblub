#include <cppunit/TestCase.h>
 #include <cppunit/TestListener.h>
 #include <cppunit/Test.h>
 #include <time.h>    // for clock()

#include "System/Logger.h"
#include "System/Config.h"
#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Scene/Light.h"
#include "Mesh/Mesh.h"
#include "Procedural/MengerSponge.h"

#include <iostream>

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
//      string c[] = { "foo", "bar" };
//      string b[2] = c;
    }
    _chronometer.end();
  }
};

class SpongeTest: public CppUnit::TestCase {
  Clock _chronometer;
public:
  SpongeTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    _chronometer.start();
//    int ar1[5] = {1,2,3,4,5}, ar2[5] = {6,7,8,9,10};
//    int ar3[10];
//    std::copy(ar1, ar1+5, ar3);
//    std::copy(ar2, ar2+5, ar3+5);
//    std::cout << ar1<< ar2<< ar3<< "\n";
//    MengerSponge * sponge = new MengerSponge(5);
//    GLfloat verts[3] = {1,2,3};
//    GLuint index[5] = {1,2,1,0,2};
//    Mesh * mesh = new Mesh();
//    mesh->addBuffer(verts, 3, "in_Vertex");
//    mesh->addElementBuffer(index);
//    mesh->setDrawType(GL_TRIANGLES);
//
//       int arr[0];
//       int size = sizeof( arr ) / sizeof( arr[0] );
//      printf("size: %d\n", size );
    _chronometer.end();
  }
};

int main() {
//  VectorTest foo("bar");
//  foo.runTest();
  SpongeTest foo2("bar");
  foo2.runTest();
}




