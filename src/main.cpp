#include "MediaLayer.h"
#include "RenderEngine.h"
#include "ShaderFactory.h"
#include "Geometry.h"
#include "Camera.h"
#include "ObjLoader.h"
#include "config.h"

/*
#include "COLLADAFWRoot.h"
#include "COLLADAFWGeometry.h"
#include "COLLADAFWNode.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWInstanceGeometry.h"
*/



MediaLayer * mediaLayer;

void initScene(){

	//Geometry::makeTetrahedron();

	//ObjLoader::Instance().readFile("monkey.obj");
	//ObjLoader::Instance().readFile("monkeySub6.obj");
	ObjLoader::Instance().readFile("monkeyMedium.obj");
	//ObjLoader::Instance().readFile("cube.obj");

	//makeCube();
	//ShaderFactory::Instance().vertexColorShader();
	//ShaderFactory::Instance().brickShader();
	//ShaderFactory::Instance().textureShader();
	//ShaderFactory::Instance().multiTextureShader();
    //ShaderFactory::Instance().coneMapShader();
    //ShaderFactory::Instance().earthShader();
	ShaderFactory::Instance().procBump();
	//ShaderFactory::Instance().relief();
	//ShaderFactory::Instance().bump1();
	//ShaderFactory::Instance().convolution();
	//ShaderFactory::Instance().ocean();

	RenderEngine::Instance().shaderProgram->linkAndUse();
	ShaderFactory::Instance().addUniforms();

}

int main(int argc, char *argv[])
{

	mediaLayer = new MediaLayer(PROGRAM_NAME, 1920, 1080);

	Camera::Instance().setParams(70.0, 0.1, 100.0);
	Camera::Instance().setAspect(16.0/9.0);
	initScene();



	mediaLayer->renderLoop();

    delete mediaLayer;

}
