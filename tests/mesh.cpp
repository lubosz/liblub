#include <cppunit/TestCase.h>

#include <QRectF>
#include "Load/MeshLoader.h"
#include "Procedural/Geometry.h"
#include "Procedural/MengerSponge.h"
#include "Application/XCB/XCBWindow.h"

class MeshTest: public CppUnit::TestCase {
public:
  MeshTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    XCBWindow * window  = new XCBWindow();

    QList<string> attributes = QList<string>() << "color";
//    attributes << "color" << "normal" << "tangent" << "bitangent" << "uv";

    Mesh * mysphere = Geometry::sphere(attributes, 11.0f, 100, 50);
    mysphere->setDrawType(GL_PATCHES);
    delete mysphere;
    Mesh * cube = Geometry::cube(attributes);
    cube->setDrawType(GL_PATCHES);
    delete cube;

    Mesh * tetrahedron = Geometry::tetrahedron(attributes);
    tetrahedron->setDrawType(GL_PATCHES);
    delete tetrahedron;

    Mesh * plane = Geometry::plane(attributes, QRectF(0,0,10,10));
    plane->setDrawType(GL_PATCHES);
    delete plane;

    QVector3D resolution = QVector3D(10.0f, 10.0f, 10.0f);
    Mesh * stars = Geometry::stars(attributes, resolution, 1.0f, 1.0f, 1.0f);
    stars->setDrawType(GL_PATCHES);
    delete stars;

    Mesh * spiral = Geometry::spiral(attributes, 30);
    spiral->setDrawType(GL_PATCHES);
    delete spiral;

    Mesh * icosahedron = Geometry::icosahedron(attributes);
    icosahedron->setDrawType(GL_PATCHES);
    delete icosahedron;

    Mesh * monkey = MeshLoader::load(attributes,"monkey.blend");
    monkey->setDrawType(GL_POINTS);
    delete monkey;

    MengerSponge * sponge = new MengerSponge(attributes, 1);
    Mesh * spongeMesh = sponge->getMesh();
    spongeMesh->setDrawType(GL_POINTS);
    delete spongeMesh;
    delete sponge;
    attributes.clear();

    delete window;
  }
};

int main() {
  MeshTest foo2("bar");
  foo2.runTest();
}




