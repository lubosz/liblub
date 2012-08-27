#include <cppunit/TestCase.h>

#include "Material/Textures.h"
#include "Material/ProcTextures.h"
#include "System/Config.h"

class TextureTest: public CppUnit::TestCase {
public:
  TextureTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    Config::Instance().load("config.xml");
    TextureFile("bunny.png", "bunny");
    QSize size = QSize(100, 100);
    DepthTexture(size, "depth");
    ShadowTexture(size, "shadow");
    ColorTexture(size, "color");
    QImage * glow = ProcTextures::makeGlow(size, 1.0f, 1.0f);
    TextureQImage(*glow, "qimage");
    CubeTextureFile("cubemaps/morning", "cube");
    delete glow;
  }
};

int main() {
  TextureTest foo2("bar");
  foo2.runTest();
}




