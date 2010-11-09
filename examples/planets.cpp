#include "Application.h"

class Planets : public Application {

public:
	Planets(){
		programName = "Planets";
  }

	void scene(){
		SceneGraph::Instance().light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));
		SceneGraph::Instance().addNode(new Node("",{2.0f,0.0f,-5.0f},"earth.obj", new PhongTexMat("Earth/MarsMap_2500x1250.jpg")));
		SceneGraph::Instance().addNode(new Node("",{-2.0f,2.0f,-5.0f},"earth.obj", new TextureMaterial("Planets/Mars.jpg")));
		SceneGraph::Instance().addNode(new Node("",{-2.0f,-2.0f,-5.0f},"earth.obj", new TextureMaterial("Planets/Mars-normal.png")));
	}
};

int main(int argc, char *argv[])
{
	Planets().run();

}
