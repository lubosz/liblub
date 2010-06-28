#include "MediaLayer.h"
#include "RenderEngine.h"
#include "ShaderFactory.h"
#include "Geometry.h"
#include "Camera.h"
#include "MeshFactory.h"
#include "config.h"

MediaLayer * mediaLayer;

void addNode(string name, string file, vector<float> position){
	SceneGraph::Instance().addNode(name,position, MeshFactory::Instance().load(file));
}

void addNode(string file, vector<float> position){
	addNode(file, file, position);
}

void addNode(Mesh * mesh, vector<float> position){
	SceneGraph::Instance().addNode("blubb",position, mesh);
}

void meshCube(string file, float cubeSize, float step){
	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			for (float z = -cubeSize/2.0; z<cubeSize/2.0; z+=step ){
				addNode(file,{x,y,z});
			}
		}
	}
}

void initScene(){

	//MeshFactory::Instance().stars();
	//MeshFactory::Instance().tetrahedron();

	//MeshFactory::Instance().loadObj("monkeySmooth.obj");
	/*
	addNode(MeshFactory::Instance().stars(),{-5,-5,-5});
	addNode(MeshFactory::Instance().tetrahedron(),{-3,0,0});
	addNode(MeshFactory::Instance().cube(),{-3,-3,-3});
	*/
	meshCube("monkeySmooth.obj", 10.0, 2.5);

	//addNode("monkeySmooth.obj",{0,0,0});
	//addNode("cube.obj",{3,1,1});

	//MeshFactory::Instance().cube();


	//MeshFactory::Instance().loadObj("monkey.obj");

	//MeshFactory::Instance().loadObj("monkeySub6.obj");
	//MeshFactory::Instance().loadObj("monkeyMedium.obj");
	//MeshFactory::Instance().loadObj("cube.obj");

	//ShaderFactory::Instance().vertexColorShader();
	//ShaderFactory::Instance().brickShader();
	//ShaderFactory::Instance().textureShader();
	//ShaderFactory::Instance().multiTextureShader();
    //ShaderFactory::Instance().coneMapShader();
    //ShaderFactory::Instance().earthShader();
	//ShaderFactory::Instance().procBump();
	//ShaderFactory::Instance().particle();
	//ShaderFactory::Instance().relief();
	//ShaderFactory::Instance().bump1();
	//ShaderFactory::Instance().convolution();
	//ShaderFactory::Instance().ocean();
	ShaderFactory::Instance().env();
	//ShaderFactory::Instance().normalcolor();
	//ShaderFactory::Instance().stars();

	RenderEngine::Instance().shaderProgram->linkAndUse();
	ShaderFactory::Instance().addUniforms();

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
