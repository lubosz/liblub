#include <cppunit/TestCase.h>

#include <QRectF>
#include "Procedural/Geometry.h"
#include "Load/MeshLoader.h"
#include "System/Config.h"
#include "Procedural/MengerSponge.h"
#include "Application/XCB/XCBWindow.h"

class MaxArraysTest: public CppUnit::TestCase {
public:
  MaxArraysTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
      XCBWindow * window  = new XCBWindow();

      for(int i = 0; i < 50; i++) {
            GLuint vao;
            glGenVertexArrays(1, &vao);
            LogDebug << "Generating Vertex Array Object #" << vao;
            glError;
            glDeleteVertexArrays(1, &vao);
      }

      delete window;
  }
};

int main() {
  MaxArraysTest foo2("bar");
  foo2.runTest();
}




