#include "Application.h"
#include "MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		width = 1920;
		height = 1200;
		programName = "WaterPlanet";
  }

	void scene(){
		SceneGraph::Instance().addNode(new Node("Planet",{0,0,-10}, 10, MeshFactory::Instance().load("earth.obj"),new OceanMat()));
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
