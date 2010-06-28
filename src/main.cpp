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
	//MeshFactory::Instance().cube();
	//MeshFactory::Instance().stars();
	//MeshFactory::Instance().loadObj("monkey.obj");
	MeshFactory::Instance().loadObj("monkeySmooth.obj");
	//MeshFactory::Instance().loadObj("monkeySub6.obj");
	//MeshFactory::Instance().loadObj("monkeyMedium.obj");
	//MeshFactory::Instance().loadObj("cube.obj");
	
	GL_POINTS,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_LINES,
#ifndef USE_GL3
	GL_POLYGON,
	GL_QUAD_STRIP,
	GL_QUADS,
#endif
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
	GL_TRIANGLES,
	GL_LINES_ADJACENCY,
	GL_LINE_STRIP_ADJACENCY,
	GL_TRIANGLES_ADJACENCY,
	GL_TRIANGLE_STRIP_ADJACENCY;


	//MeshFactory::Instance().meshes[0]->setDrawType(GL_TRIANGLE_STRIP);
	MeshFactory::Instance().meshes[0]->setDrawType(GL_TRIANGLES);
	//MeshFactory::Instance().meshes[0]->setDrawType(GL_LINES);
	//MeshFactory::Instance().meshes[0]->setDrawType(GL_POINTS);

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
	//ShaderFactory::Instance().env();
	//ShaderFactory::Instance().normalcolor();
	ShaderFactory::Instance().stars();

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
