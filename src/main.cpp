#include "MediaLayer.h"
#include "RenderEngine.h"
#include "ShaderFactory.h"
#include "Geometry.h"
#include "Camera.h"
#include "MeshFactory.h"
#include "config.h"

MediaLayer * mediaLayer;

void initScene(){

	//MeshFactory::Instance().tetrahedron();
	//MeshFactory::Instance().loadObj("monkey.obj");
	//MeshFactory::Instance().loadObj("monkeySub6.obj");
//MeshFactory::Instance().loadObj("monkeyMedium.obj");
	MeshFactory::Instance().loadObj("cube.obj");
	
	GL_POINTS,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_LINES,
	GL_POLYGON,
	GL_QUAD_STRIP,
	GL_QUADS,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
	GL_TRIANGLES,
	GL_LINES_ADJACENCY,
	GL_LINE_STRIP_ADJACENCY,
	GL_TRIANGLES_ADJACENCY,
	GL_TRIANGLE_STRIP_ADJACENCY;


	MeshFactory::Instance().meshes[0]->setDrawType(GL_LINES_ADJACENCY);
	//ObjLoader::Instance().readFile("monkey.obj");
	//ObjLoader::Instance().readFile("monkeySub6.obj");
	//ObjLoader::Instance().readFile("monkeyMedium.obj");
	//ObjLoader::Instance().readFile("cube.obj");

	//makeCube();
	//ShaderFactory::Instance().vertexColorShader();
	//ShaderFactory::Instance().brickShader();
	ShaderFactory::Instance().textureShader();
	//ShaderFactory::Instance().multiTextureShader();
    //ShaderFactory::Instance().coneMapShader();
    //ShaderFactory::Instance().earthShader();
	//ShaderFactory::Instance().procBump();
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
