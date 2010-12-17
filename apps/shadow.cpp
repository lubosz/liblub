#include "Application.h"

class Example : public Application {

public:
	Example(){
		programName = "Shadow Mapping";
  }

	void scene(){

		SceneGraph::Instance().light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));


/*
		Material * paper = new PhongTexMat("Paper_Texture_by_Spiteful_Pie_Stock.jpg");


		Material * phun = new NormalMapMaterial("spectrum.png","bump/direction_w_ev_n.jpg");

		Material * cubeMap = new EnvMat();
		Material * texture = new TextureMaterial("bunny.png");
*/
		/*
		Material * bump = new NormalMapMaterial("bump/brickwork-texture.jpg","bump/brickwork_normal-map.jpg");
		Material * green = new PhongColorMat(QVector3D(0,1,0));
		Material * sphereMap = new SphereMap("spectrum.png", "spheremaps/spheremap_bar.jpg");
*/



		//Texture * shadowMap = RenderEngine::Instance().shadowSequence->renderPasses[0]->targetTexture;

		Material * relief = new ReliefMat();
		Material * monkey = new Ubershader("bump/masonry-wall-texture.jpg","monkey-occlusion.png", "bump/masonry-wall-normal-map.jpg");
		Material * radio = new Ubershader("bump/masonry-wall-texture.jpg","radio-occlusion.png", "bump/masonry-wall-normal-map.jpg");
		Material * ground = new Ubershader("bump/brickwork-texture.jpg","plane-occlusion.png", "bump/brickwork_normal-map.jpg");
		//Material * bump = new NormalMapMaterial("bump/brickwork-texture.jpg","bump/brickwork_normal-map.jpg");

		Node * plane = new Node("Plane",{0,-7,0}, 20, MeshFactory::load("plane.blend"),ground);
		plane->setReceiveShadows(true);
		plane->setCastShadows(false);
		SceneGraph::Instance().addNode(plane);


		//SceneGraph::Instance().addNode(new Node("Monkey",{5,0,-5}, 1, MeshFactory::Instance().loadAssimp("monkeyBig.obj"),paper));
		//SceneGraph::Instance().addNode(new Node("Sphere",{0,-2,-7}, 3, MeshFactory::Instance().load("earth.obj"),bump));
		SceneGraph::Instance().addNode(new Node("bump",{3,-2,-5}, 1, MeshFactory::load("cube.obj"),relief));
		SceneGraph::Instance().addNode(new Node("bump",{3,-2,-9}, 1, MeshFactory::load("cube.obj"),ground));
		//SceneGraph::Instance().addNode(new Node("Radio",{0,2,-5}, 1, MeshFactory::Instance().loadAssimp("cube.obj"),phun));

		SceneGraph::Instance().addNode(new Node("Radio",{-5,-5,-5}, .5, MeshFactory::load("radio.blend"),radio));
		//
		//SceneGraph::Instance().addNode(MeshFactory::Instance().spiral(),{0,-15,0},new StarMat());
	}
};

int main(int argc, char *argv[])
{
	Example().run();

}
