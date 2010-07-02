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
	Mesh * mesh = MeshFactory::Instance().load(file);

	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			for (float z = -cubeSize/2.0; z<cubeSize/2.0; z+=step ){
				addNode(mesh,{x,y,z}, material);
			}
		}
	}
}

void meshCube(string file, float cubeSize, float step, vector<Material*> materials){
	Mesh * mesh = MeshFactory::Instance().load(file);
	unsigned position = 0;
	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			for (float z = -cubeSize/2.0; z<cubeSize/2.0; z+=step ){
				addNode(mesh,{z,x,y}, materials.at(position%materials.size()));
				position++;
			}
		}
	}
}

void meshPlane(string file, float cubeSize, float step, vector<Material*> materials){
	Mesh * mesh = MeshFactory::Instance().load(file);
	unsigned position = 0;
	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			addNode(mesh,{x,y,-5+x}, materials.at(position%materials.size()));
			position++;
		}
	}
}

void niceScene(){
	vector<Material*> textureMats = {
			new TextureMaterial("bunny.png"),
			new MultiTextureMaterial(),
			new EarthMaterial(),
			new EnvMat(),
			new NormalColorMat(),
			new BumpMaterial1(),
			new BrickMaterial(),
			new ProcBumpMaterial(),
			new OceanMat()
	};
	meshPlane("monkeySmooth.obj", 10, 3, textureMats);
	addNode(MeshFactory::Instance().stars(),{0,-50,-50},new StarMat());
}

void bufferTest(){
	Material* material= new VertexMaterial();
	Material* material1= new NormalColorMat();
	Material* material2= new TextureMaterial("bunny.png");
	Mesh * plane = MeshFactory::Instance().plane();
	addNode(plane,{-2,0,-5}, material);
	addNode(plane,{0,0,-5}, material1);
	addNode(plane,{2,0,-5}, material2);
}

void planetTest(){
	vector<Material*> textureMats = {
			new TextureMaterial("Earth/MarsMap_2500x1250.jpg"),
			new TextureMaterial("Earth/EarthMap_2500x1250.jpg"),
			new TextureMaterial("Earth/MoonMap_2500x1250.jpg"),
			new NormalColorMat(),
			//new BumpMaterial1(),
			new EnvMat(),
			new BrickMaterial(),
			new ProcBumpMaterial(),
			new OceanMat()
	};
	meshPlane("earth.obj", 10, 3, textureMats);
}

void initScene(){
	//bufferTest();
	//planetTest();
	//niceScene();
	//addNode("earth.obj",{0,0,-5}, new VertexMaterial());

	//addNode("remesh/shaderX7.obj",{0,0,-5}, new TextureMaterial("remesh/multi.tga"));
	addNode("remesh/shaderX7.obj",{0,0,-5}, new RemeshMat());
	//addNode("remesh/smoothShaderX7.obj",{2,0,-5}, new TextureMaterial("remesh/shaderX7.tga"));
	//addNode("remesh/multi.obj",{2,0,-10}, new TextureMaterial("remesh/multi.tga"));

/*
 vector<Material*> pointMats = {
			//new StarMat()
			//new ParticleMat(),
	};

	 vector<Material*> bugMats = {
	 				//Multitexture bugs!

				new MultiTextureMaterial(),

				new ReliefMat(),
				new BumpMaterial1(),
				new BumpMaterial2(),
				//new ConeMapMaterial() // Dont do this


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

	mediaLayer->renderLoop();

    delete mediaLayer;

}
