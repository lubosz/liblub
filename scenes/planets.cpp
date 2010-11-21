#include "Application.h"

class Planets : public Application {

public:
	Planets(){
		programName = "Planets";
  }

	void scene(){
		SceneGraph::Instance().light = new Light(QVector3D(-15.2, 19.4, 6.5),QVector3D(1,-5,-1));
		Texture * shadowMap = RenderEngine::Instance().shadowSequence->renderPasses[0]->targetTexture;

		Material * ground = new Ubershader(shadowMap, "bump/brickwork-texture.jpg","plane-occlusion.png", "bump/brickwork_normal-map.jpg");

		Mesh * sphere = MeshFactory::load("earth.obj");

		SceneGraph::Instance().addNode(
				new Node(
						"mars",
						{-10,-10,-10},
						10.0f,
						sphere,
						new Ubershader(shadowMap, "Planets/Mars.jpg","plane-occlusion.png", "Planets/Mars-normal.png")
				)
		);

		SceneGraph::Instance().addNode(
				new Node(
						"waterplanet",
						{-10,5,.05},
						1,
						sphere,
						new OceanMat()
				)
		);


		Node * skydome = new Node(
				"skydome",
				{0,0,0},
				500,
				sphere,
				new TextureMaterial("Planets/TychoSkymap.jpg")
		);
		SceneGraph::Instance().addNode(skydome);
	}
};

int main(int argc, char *argv[])
{
	Planets().run();

}
