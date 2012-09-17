#include <cppunit/TestCase.h>
#include "System/Logger.h"
#include "System/Config.h"
#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Scene/Light.h"
#include "Mesh/Mesh.h"
#include "Application/XCB/XCBWindow.h"
#include "Shader/Shaders.h"

class NodeTest : public CppUnit::TestCase {
public:
  NodeTest( std::string name ) : CppUnit::TestCase( name ) {}

  void runTest() {
      QList<string> attributes = QList<string>();
      attributes.push_back("uv");
    XCBWindow * window  = new XCBWindow();
    Camera * camera = new Camera();
    Light * light = new Light(QVector3D(0, 0, 0),QVector3D(0, -5, 0));
    Mesh * mesh = new Mesh(attributes); //Geometry::plane(attributes, QRectF(0.5,0,0.5,1)
    ShaderProgram * shader = new VertFragProgram("Texture/texture",attributes);

    Node * node = new Node("Test", QVector3D(0,0,0), 1, mesh, shader);
    delete camera;
    delete light;
    delete node;

    delete window;
  }
};


int main() {
  NodeTest foo("bar");
  foo.runTest();
}
