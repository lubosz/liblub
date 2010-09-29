#include "Application.h"

class Example : public Application {

public:
	Example(){
		width = 1920;
		height = 1200;
		programName = "Phong Shading";
  }

	void scene(){
		SceneGraph::Instance().addNode(new Node("Monkey",{0,-5,-5}, 1, MeshFactory::Instance().load("monkeySmooth.obj"),new PhongColorMat()));
	}
};

int main(int argc, char *argv[])
{
	Example().run();

}
