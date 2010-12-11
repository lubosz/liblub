#include "Application.h"

class Planets : public Application {

public:
	Planets(){
		programName = "Geomerty Clipmap";
  }

	void scene(){
		SceneGraph::Instance().light = new Light(QVector3D(-15.2, 19.4, 6.5),QVector3D(1,-5,-1));

		SceneGraph::Instance().addNode(
				new Node(
						"mars",
						{0,-10,0},
						10.0f,
						MeshFactory::load("clipmap.obj"),
						//MeshFactory::load("hugePlane.obj"),
						new ClipMapMat("Planets/Mars.jpg", "noise.png","noise-normal.png")
						//new HeightMapMat("bunny.png", "bunny.png")
				)
		);

	}
};

int main(int argc, char *argv[])
{
	Planets().run();

}
