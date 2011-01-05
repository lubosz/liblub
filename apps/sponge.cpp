#include "System/Application.h"
#include "Mesh/MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		programName = "Sponge";
  }

	void scene(){
		SceneGraph::Instance().light = new Light(QVector3D(-2.5, 21.5, -5.2),QVector3D(1,-5,0));

	    Material * material = new UbershaderColor(QVector3D(1,1,1));

	    for (int i = 0; i < 5; i++){
	    	MengerSponge * sponge = new MengerSponge(i);
			Node * node = new Node(
					"sponge",
					{float(i-2.5),float(i*3-6),-5},
					1,
					sponge->getMesh(),
					material
			);
			SceneGraph::Instance().addNode(node);

	    }

		Node * plane = new Node("Plane",{0,-7,0}, 20, MeshFactory::load("plane.obj"),material);
		plane->setReceiveShadows(true);
		plane->setCastShadows(false);
		SceneGraph::Instance().addNode(plane);

	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
