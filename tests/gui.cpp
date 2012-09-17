#include "System/Config.h"
#include "System/FontOverlay.h"
#include <iostream>
#include <QApplication>
#include <cppunit/TestCase.h>
#include "Application/XCB/XCBWindow.h"


class TextTest: public CppUnit::TestCase {
public:
  TextTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
      Config::Instance();
      XCBWindow * window  = new XCBWindow();
      FontOverlay gui;
      gui.init();
      delete window;
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  TextTest foo("bar");
  foo.runTest();
}




