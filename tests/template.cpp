#include <cppunit/TestCase.h>
#include "System/Logger.h"
#include "System/Config.h"
#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Scene/Light.h"
#include "Mesh/Mesh.h"
#include "System/TemplateEngine.h"

class TemplateTest : public CppUnit::TestCase {
public:
  TemplateTest( std::string name ) : CppUnit::TestCase( name ) {}

  void runTest() {
    printf("Vertex Shader:\n");
    TemplateEngine::Instance().print("Atmo/Ground.vert");
//    TemplateEngine::Instance().render("Ubershader.vert");
    printf("Fragment Shader:\n");
    TemplateEngine::Instance().print("Atmo/Ground.frag");

    CPPUNIT_ASSERT( 0 == 0 );
  }
};


int main() {
  TemplateTest foo("bar");
  foo.runTest();
}
