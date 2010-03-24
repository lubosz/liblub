#include "MediaLayer.h"
#include "RenderEngine.h"
#include "ShaderFactory.h"
#include "Geometry.h"

#define PROGRAM_NAME "Cone Step Mapping"

MediaLayer * mediaLayer;

void initScene(){

	Geometry::makeTetrahedron();

	//makeCube();
	//vertexColorShader();
	//brickShader();
	//textureShader();
	ShaderFactory::Instance().multiTextureShader();

	//coneMapShader();

	RenderEngine::Instance().shaderProgram->linkAndUse();
	ShaderFactory::Instance().addUniforms();

}

int main(int argc, char *argv[])
{

	mediaLayer = new MediaLayer(PROGRAM_NAME, 1024, 576);

	initScene();

	mediaLayer->renderLoop();

    delete mediaLayer;

}
