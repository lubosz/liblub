#include "System/Config.h"
#include "Shader/Shaders.h"
#include <cppunit/TestCase.h>
#include "Application/XCB/XCBWindow.h"


class TextTest: public CppUnit::TestCase {
public:
  TextTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {

      QList<string> attributes = QList<string>() << "uv";

      XCBWindow * window  = new XCBWindow();
      VertFragProgram * simpleProgram = new VertFragProgram("Texture/texture",attributes);

      ShaderProgram * program = new ShaderProgram("A Program");

      delete program;
      delete simpleProgram;
      delete window;
  }
};

int main() {
  TextTest foo("bar");
  foo.runTest();
}




