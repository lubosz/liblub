#include "Application.h"

class Example : public Application {

public:
	Example(){
		programName = "Spiral";
  }

	void scene(){
		SceneGraph::Instance().light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));
		Node * plane = new Node(
				"Planet",
				{0,0,0},
				.2,
				MeshFactory::load("monkey.obj"),
				new TextureMaterial("bunny.png")
		);
		SceneGraph::Instance().addNode(plane);
		SceneGraph::Instance().addNode(new Node("Spiral",{0,0,0}, MeshFactory::spiral(),new Simple("Particle/stars")));
	}
};

int main(int argc, char *argv[])
{
	Example().run();

}