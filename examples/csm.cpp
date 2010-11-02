#include "Application.h"
#include "MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		programName = "CSM";
  }

	void scene(){

		SceneGraph::Instance().light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));

		Material * material = new ConeMapMaterial();
		//Material * bump = new NormalMapMaterial("bump/brickwork-texture.jpg","bump/brickwork_normal-map.jpg");
		//SceneGraph::Instance().addNode(new Node("Room",{0,0,0}, .05, MeshFactory::Instance().loadAssimp("myroom.obj"),bump));
		SceneGraph::Instance().addNode(new Node("Room",{0,-3,-2}, 1, MeshFactory::load("plane.obj"),material));
		//SceneGraph::Instance().addNode(new Node("Room",{0,-3,-2}, 1, MeshFactory::Instance().loadAssimp("myroom.obj"),material));
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
