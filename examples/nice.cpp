#include "Application.h"
#include "MengerSponge.h"

class Example : public Application {

public:
	Example(){
		width = 1920;
		height = 1200;
		programName = "Example";
  }

	void scene(){
		Light * light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));
		SceneGraph::Instance().light = light;

		vector<Material*> textureMats = {

				//new TextureMaterial("bunny.png"),
				//new MultiTextureMaterial(),
				//new EarthMaterial(),
				new EnvMat(),
				new NormalColorMat(),
				//new BrickMaterial(),
				//new ProcBumpMaterial(),
				new OceanMat(),
				new NormalMapMaterial("bump/brickwork-texture.jpg","bump/brickwork_normal-map.jpg"),
				new PhongColorMat(QVector3D(0,1,0)),
				new SphereMap("spectrum.png", "spheremaps/spheremap_bar.jpg"),

				new PhongTexMat("Paper_Texture_by_Spiteful_Pie_Stock.jpg"),

				new MandelMat()
		};
		SceneGraph::Instance().meshPlane("monkey.blend", 10, 3, textureMats);
		Material * starMat = new StarMat();
		SceneGraph::Instance().addNode(new Node("",{0,-50,-50},MeshFactory::Instance().stars(),starMat));
		//SceneGraph::Instance().addNode(new Node("Spiral",{0,0,0}, MeshFactory::Instance().spiral(),starMat));

		MengerSponge * sponge = new MengerSponge(3);

		Material * material = new PhongColorMat(QVector3D(.5,1,1));
	    SceneGraph::Instance().addNode(new Node("", QVector3D(10,0,-10), 3.0, sponge->getMesh(),material));
	}
};

int main(int argc, char *argv[])
{
	Example().run();

}
