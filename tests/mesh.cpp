#include <cppunit/TestCase.h>

#include <QRectF>
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "System/Config.h"
#include "Mesh/MengerSponge.h"

class MeshTest: public CppUnit::TestCase {
public:
  MeshTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    QList<string> attributes;
    attributes.push_back("uv");
    Config::Instance().load("config.xml");
    Mesh * sphere = Geometry::sphere(attributes, 11.0f, 100, 50);
    sphere->setDrawType(GL_PATCHES);
    Mesh * cube = Geometry::cube(attributes);
    cube->setDrawType(GL_PATCHES);
    Mesh * tetrahedron = Geometry::tetrahedron(attributes);
    tetrahedron->setDrawType(GL_PATCHES);
    Mesh * plane = Geometry::plane(attributes, QRectF(0,0,10,10));
    plane->setDrawType(GL_PATCHES);
    vector<float> resolution = { 10.0f, 10.0f, 10.0f };
    Mesh * stars = Geometry::stars(attributes, resolution, 1.0f, 1.0f, 1.0f);
    stars->setDrawType(GL_PATCHES);
    Mesh * spiral = Geometry::spiral(attributes, 30);
    spiral->setDrawType(GL_PATCHES);
    Mesh * icosahedron = Geometry::icosahedron(attributes);
    icosahedron->setDrawType(GL_PATCHES);
    Mesh * monkey = MeshLoader::load(attributes,"monkey.blend");
    monkey->setDrawType(GL_POINTS);
    MengerSponge * sponge = new MengerSponge(attributes, 3);
    Mesh * spongeMesh = sponge->getMesh();
    spongeMesh->setDrawType(GL_POINTS);
  }
};

int main() {
  MeshTest foo2("bar");
  foo2.runTest();
}




