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
    DepthTexture(100, 100, "depth");
    ShadowTexture(100, 100, "shadow");
    ColorTexture(100, 100, "color");
    QImage * glow = ProcTextures::makeGlow(QSize(100,100), 1.0f, 1.0f);
    TextureQImage(glow, "qimage");
    SplatTexture("splat", 200);
    CubeTextureFile("cubemaps/morning", "cube");
  }
};

int main() {
  TextureTest foo2("bar");
  foo2.runTest();
}




