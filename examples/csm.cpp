#include "Application.h"
#include "MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		width = 1920;
		height = 1200;
		programName = "CSM";
  }

	void scene(){
		Light * light = new Light(QVector4D(0.3, 6.6, -3.3, 1.0),QVector3D(1,-5,-1));

		SceneGraph::Instance().light = light;

		Material * material = new ConeMapMaterial();
		//Material * bump = new NormalMapMaterial("bump/brickwork-texture.jpg","bump/brickwork_normal-map.jpg");
		//SceneGraph::Instance().addNode(new Node("Room",{0,0,0}, .05, MeshFactory::Instance().loadAssimp("myroom.obj"),bump));
		SceneGraph::Instance().addNode(new Node("Room",{0,-3,-2}, 1, MeshFactory::Instance().load("plane.blend"),material));
		//SceneGraph::Instance().addNode(new Node("Room",{0,-3,-2}, 1, MeshFactory::Instance().loadAssimp("myroom.obj"),material));
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
