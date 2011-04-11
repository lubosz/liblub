#include <cppunit/TestCase.h>
#include "System/Logger.h"
#include "System/Config.h"
#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Scene/Light.h"
#include "Material/Materials.h"
#include "Mesh/Mesh.h"
#include "System/TemplateEngine.h"

class TemplateTest : public CppUnit::TestCase {
public:
  TemplateTest( std::string name ) : CppUnit::TestCase( name ) {}

  void runTest() {
    TemplateEngine::Instance();
    CPPUNIT_ASSERT( 0 == 0 );
  }
};


int main() {
  TemplateTest foo("bar");
  foo.runTest();
}
