#include "Application.h"
#include "Light.h"

class Example : public Application {

public:
	Example(){
		width = 1920;
		height = 1200;
		programName = "Phong Shading";
  }

	void scene(){
		Light * light = new Light(QVector4D(0.3, 6.6, -3.3, 1.0));

		light->setDirection(QVector3D(1,-5,-1));
		SceneGraph::Instance().light = light;

		Material * phongMaterial = new PhongTexMat("Paper_Texture_by_Spiteful_Pie_Stock.jpg");
		SceneGraph::Instance().addNode(new Node("Monkey",{5,0,-5}, 1, MeshFactory::Instance().load("monkeyBig.obj"),phongMaterial));
		SceneGraph::Instance().addNode(new Node("Sphere",{0,3,-5}, 1, MeshFactory::Instance().load("earth.obj"),phongMaterial));
		SceneGraph::Instance().addNode(new Node("Radio",{0,-5,-5}, .5, MeshFactory::Instance().load("radio.obj"),phongMaterial));
		SceneGraph::Instance().addNode(new Node("Radio",{0,-7,0}, 3, MeshFactory::Instance().load("terrain.obj"),phongMaterial));
	}
};

int main(int argc, char *argv[])
{
	Example().run();

}
