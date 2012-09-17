#include <cppunit/TestCase.h>

#include "System/Config.h"
#include "Application/XCB/XCBWindow.h"

class MeshTest: public CppUnit::TestCase {
public:
  MeshTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    XCBWindow * window  = new XCBWindow();
    delete window;
  }
};

int main() {
  MeshTest foo2("bar");
  foo2.runTest();
}




