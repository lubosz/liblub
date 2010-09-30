#include "Application.h"

class Example : public Application {

public:
	Example(){
		width = 1920;
		height = 1200;
		programName = "Phong Shading";
  }

	void scene(){
		Material * phongMaterial = new PhongColorMat();
		SceneGraph::Instance().addNode(new Node("Monkey",{5,0,-5}, 1, MeshFactory::Instance().load("monkeyBig.obj"),phongMaterial));
		SceneGraph::Instance().addNode(new Node("Sphere",{0,5,-5}, 1, MeshFactory::Instance().load("earth.obj"),phongMaterial));
		SceneGraph::Instance().addNode(new Node("Radio",{0,-5,-5}, .5, MeshFactory::Instance().load("radio.obj"),phongMaterial));
	}
};

int main(int argc, char *argv[])
{
	Example().run();

}
