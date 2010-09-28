#include "Application.h"

class Example : public Application {

public:
	Example(){
		width = 800;
		height = 600;
		programName = "Example";
  }

	void scene(){
		vector<Material*> textureMats = {
				new TextureMaterial("bunny.png"),
				new MultiTextureMaterial(),
				//new EarthMaterial(),
				new EnvMat(),
				new NormalColorMat(),
				new BrickMaterial(),
				new ProcBumpMaterial(),
				new OceanMat()
		};
		SceneGraph::Instance().meshPlane("monkeySmooth.obj", 10, 3, textureMats);
		SceneGraph::Instance().addNode(MeshFactory::Instance().stars(),{0,-50,-50},new StarMat());
	}
};

int main(int argc, char *argv[])
{
	Example().run();

}
