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
		//Material * material = new ConeMapMaterial();
		Material * material = new TextureMaterial("bunny.png");
		//Node * sphere = new Node("Planet",{0,0,-10}, 10, MeshFactory::Instance().load("earth.obj"),material);
		Node * plane = new Node(
				"Planet",
				{0,0,0},
				.01,
				//MeshFactory::Instance().load("myroom-quads.obj"),
				MeshFactory::Instance().plane(),
				material
		);
		SceneGraph::Instance().addNode(plane);
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
