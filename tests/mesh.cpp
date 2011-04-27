#include <cppunit/TestCase.h>

#include <QRectF>
#include "Mesh/Geometry.h"

class MeshTest: public CppUnit::TestCase {
public:
  MeshTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    Mesh * sphere = Geometry::sphere(11.0f, 100, 50);
    sphere->setDrawType(GL_PATCHES);
    Mesh * cube = Geometry::cube();
    cube->setDrawType(GL_PATCHES);
    Mesh * tetrahedron = Geometry::tetrahedron();
    tetrahedron->setDrawType(GL_PATCHES);
    Mesh * plane = Geometry::plane(QRectF(0,0,10,10));
    plane->setDrawType(GL_PATCHES);
    vector<float> resolution = { 10.0f, 10.0f, 10.0f };
    Mesh * stars = Geometry::stars(resolution, 1.0f, 1.0f, 1.0f);
    stars->setDrawType(GL_PATCHES);
    Mesh * spiral = Geometry::spiral(30);
    spiral->setDrawType(GL_PATCHES);
    Mesh * icosahedron = Geometry::icosahedron();
    icosahedron->setDrawType(GL_PATCHES);
  }
};

int main() {
  MeshTest foo2("bar");
  foo2.runTest();
}




