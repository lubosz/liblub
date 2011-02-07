#include <cppunit/TestCase.h>
#include "System/Logger.h"
#include "System/Config.h"
#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Scene/Light.h"
#include "Material/Materials.h"
#include "Mesh/Mesh.h"


class NodeTest : public CppUnit::TestCase {
public:
  NodeTest( std::string name ) : CppUnit::TestCase( name ) {}

  void runTest() {
//    Config::Instance().load("config.xml");
//    Camera * camera = new Camera();
//    Light * light = new Light(QVector3D(0, 0, 0),QVector3D(0, -5, 0));
//    Material * material = new EmptyMat();
//    Mesh * mesh = new Mesh();
//    Node * node = new Node("", QVector3D(0,0,0), 1, mesh, material);

    CPPUNIT_ASSERT( 0 == 0 );
  }
};


int main() {
  NodeTest foo("bar");
  foo.runTest();
}
