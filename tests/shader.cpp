#include "System/Config.h"
#include "Material/Shaders.h"
//#include <QApplication>
#include <cppunit/TestCase.h>
#include "Window/XCB/XCBWindow.h"


class TextTest: public CppUnit::TestCase {
public:
  TextTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {

      QList<string> attributes = QList<string>() << "uv";

      Config::Instance().load("config.xml");
      XCBWindow * window  = new XCBWindow();
      SimpleProgram * simpleProgram = new SimpleProgram("Texture/font",attributes);

      ShaderProgram * program = new ShaderProgram();

      delete program;
      delete simpleProgram;
      delete window;
  }
};

int main() {
//  QApplication app(argc, argv);
  TextTest foo("bar");
  foo.runTest();
}




