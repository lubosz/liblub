#include "Application.h"
#include "MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		width = 1920;
		height = 1200;
		programName = "Sponge";
  }

	void scene(){
			MengerSponge * sponge = new MengerSponge(0);
			SceneGraph::Instance().addNode(sponge->getMesh(),{0,0,-5}, new GeomTest());
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
