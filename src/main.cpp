#include "MediaLayer.h"
#include "RenderEngine.h"
#include "ShaderFactory.h"
#include "Geometry.h"
#include "Camera.h"

/*
#include "COLLADAFWRoot.h"
#include "COLLADAFWGeometry.h"
#include "COLLADAFWNode.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWInstanceGeometry.h"
*/

#define PROGRAM_NAME "Cone Step Mapping"

MediaLayer * mediaLayer;

void initScene(){

	Geometry::makeTetrahedron();

	//makeCube();
	//ShaderFactory::Instance().vertexColorShader();
	//ShaderFactory::Instance().brickShader();
	//ShaderFactory::Instance().textureShader();
	//ShaderFactory::Instance().multiTextureShader();
    ShaderFactory::Instance().coneMapShader();
    //ShaderFactory::Instance().earthShader();
	//ShaderFactory::Instance().procBump();
	//ShaderFactory::Instance().relief();
	//ShaderFactory::Instance().bump1();
	//ShaderFactory::Instance().convolution();

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
