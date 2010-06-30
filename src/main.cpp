#include "MediaLayer.h"
#include "RenderEngine.h"
#include "Geometry.h"
#include "Camera.h"
#include "MeshFactory.h"
#include "config.h"
#include "MaterialTypes.h"

MediaLayer * mediaLayer;

void addNode(string name, string file, vector<float> position, Material * material){
	SceneGraph::Instance().addNode(name,position, MeshFactory::Instance().load(file),material);
}

void addNode(string file, vector<float> position, Material * material){
	addNode(file, file, position, material);
}

void addNode(Mesh * mesh, vector<float> position, Material * material){
	SceneGraph::Instance().addNode("blubb",position, mesh, material);
}

void meshCube(string file, float cubeSize, float step, Material * material){
	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			for (float z = -cubeSize/2.0; z<cubeSize/2.0; z+=step ){
				addNode(file,{x,y,z}, material);
			}
		}
	}
}

void meshCube(string file, float cubeSize, float step, vector<Material*> materials){
	unsigned position = 0;
	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			for (float z = -cubeSize/2.0; z<cubeSize/2.0; z+=step ){
				addNode(file,{z,x,y}, materials.at(position%materials.size()));
				position++;
			}
		}
	}
}

void niceScene(){
	vector<Material*> workMaterials = {
			//new TextureMaterial(),
			new EarthMaterial(),
			new EnvMat(),
			new NormalColorMat(),


	};

	meshCube("monkeySmooth.obj", 5, 2, workMaterials);
	addNode(MeshFactory::Instance().stars(),{50,50,50},new StarMat());
}

void multiTexScene(){
	vector<Material*> textureMats = {
			new TextureMaterial(),
			new EarthMaterial(),
			new EnvMat(),
			new NormalColorMat(),
			new MultiTextureMaterial()


	};

	meshCube("monkeySmooth.obj", 10, 3, textureMats);
}

void initScene(){
	//addNode("monkeySmooth.obj",{0,0,0},new MultiTextureMaterial());
	//niceScene();
	multiTexScene();

/*
 vector<Material*> pointMats = {
			//new StarMat()
			//new ParticleMat(),
	};

	 vector<Material*> bugMats = {
	 				//Multitexture bugs!

				new MultiTextureMaterial(),
				new OceanMat(), //Alone
				new ReliefMat(),
				new BumpMaterial1(),
				new BumpMaterial2(),
				new ConeMapMaterial() // Dont do this


		};

	 vector<Material*> worksAlone = {
				new ConvolutionMaterial(),
				new ProcBumpMaterial(),
				new BrickMaterial(),
		};
	 */
}

int main(int argc, char *argv[])
{
	unsigned width = 1920;
	unsigned height = 1080;

	mediaLayer = new MediaLayer(PROGRAM_NAME, width, height);

	Camera::Instance().setParams(70.0, 0.1, 100.0);
	Camera::Instance().setAspect(float(width)/float(height));
	initScene();
	SceneGraph::Instance().initUniforms();


	mediaLayer->renderLoop();

    delete mediaLayer;

}
