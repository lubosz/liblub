#include <cppunit/TestCase.h>

#include <QRectF>
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "System/Config.h"
#include "Mesh/MengerSponge.h"
#include "Window/XCB/XCBWindow.h"

class MaxArraysTest: public CppUnit::TestCase {
public:
  MaxArraysTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
      Config::Instance().load("config.xml");
      XCBWindow * window  = new XCBWindow();

      for(int i = 0; i < 50; i++) {
            GLuint vao;
            glGenVertexArrays(1, &vao);
            LogDebug << "Generating Vertex Array Object #" << vao;
            glError;
      }

      delete window;
  }
};

int main() {
  MaxArraysTest foo2("bar");
  foo2.runTest();
}




