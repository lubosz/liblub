#include "Application.h"

class Example : public Application {

public:
	Example(){
		width = 1920;
		height = 1200;
		programName = "Example";
  }

	void scene(){
		Light * light = new Light(QVector4D(0.3, 6.6, -3.3, 1.0));

		light->setDirection(QVector3D(1,-5,-1));
		SceneGraph::Instance().light = light;

		vector<Material*> textureMats = {

				new TextureMaterial("bunny.png"),
				new MultiTextureMaterial(),
				//new EarthMaterial(),
				new EnvMat(),
				new NormalColorMat(),
				new BrickMaterial(),
				new ProcBumpMaterial(),
				new OceanMat(),

				new MandelMat()
		};
		SceneGraph::Instance().meshPlane("monkeySmooth.obj", 10, 3, textureMats);
		SceneGraph::Instance().addNode(new Node("",{0,-50,-50},MeshFactory::Instance().stars(),new StarMat()));
	}
};

int main(int argc, char *argv[])
{
	Example().run();

}
