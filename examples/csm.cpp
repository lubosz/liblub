#include "Application.h"
#include "MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		width = 1920;
		height = 1200;
		programName = "CMS";
  }

	void scene(){
		Material * material = new ConeMapMaterial();
		//Node * sphere = new Node("Planet",{0,0,-10}, 10, MeshFactory::Instance().load("earth.obj"),material);
		Node * plane = new Node("Planet",{0,0,-10}, 10, MeshFactory::Instance().plane(),material);
		SceneGraph::Instance().addNode(plane);
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
