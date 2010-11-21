#include "Application.h"
#include "MengerSponge.h"
#include "Geometry.h"

class Sponge : public Application {

public:
	Sponge(){
		programName = "Stars";
  }

	void scene(){

		SceneGraph::Instance().light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));

		vector<float> resolution = {10,10,10};
		Mesh * mesh = Geometry::makeStars(resolution,1.0,0.0,5);
		mesh->setDrawType(GL_POINTS);

		Material * starMat = new Simple("Particle/stars");
		SceneGraph::Instance().addNode(
				new Node(
						"Stars",
						{0,-50,-50},
						10,
						mesh,
						starMat
						)
		);
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
