#include <cppunit/TestCase.h>
 #include <cppunit/TestListener.h>
 #include <cppunit/Test.h>
#include <QApplication>

#include "System/GUI.h"

#include <iostream>

class TextTest: public CppUnit::TestCase {
public:
  TextTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    GUI gui;
    gui.drawText("Heloww!");
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  TextTest foo("bar");
  foo.runTest();
}




