#include "Application.h"
#include "MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		programName = "Sponge";
  }

	void scene(){
		Light * light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));

		SceneGraph::Instance().light = light;

		MengerSponge * sponge = new MengerSponge(4);

		Material * material = new PhongColorMat(QVector3D(1,1,1));
	    SceneGraph::Instance().addNode(new Node("", QVector3D(0,0,-10), 2.0, sponge->getMesh(),material));

	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
