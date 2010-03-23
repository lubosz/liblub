#include <boost/multi_array.hpp>

#include "MediaLayer.h"
#include "RenderEngine.h"
#include "ShaderProgram.h"
#include "Mesh.h"

#define PROGRAM_NAME "Cone Step Mapping"

MediaLayer * mediaLayer;
RenderEngine * renderEngine;

void makeTetrahedron(){
	vector<GLfloat> vertices = {
			1.0, -1.0, -1.0,
			1.0, -1.0, 1.0,
			-1.0, -1.0, 0.0,
			0.0, 1.0, 0.0
	};

	vector<GLfloat> vertexColors = {
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0
	};

	vector<GLfloat> normals = {
			0.4, -0.3, -0.8,
			0.4, -0.3, 0.8,
			-0.8, -0.5, 0.0,
			0.0, 0.9, 0.0
	};

	vector<GLubyte> indicies = { 0, 1, 2, 3, 0, 1 };

	Mesh * tetrahedron = new Mesh(vertices,vertexColors,normals,indicies);

}

void makeCube(){

	vector<GLfloat> vertices = {
			1.0, 1.0, 1.0,
			1.0, -1.0, 1.0,
			-1.0, -1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0,
			1.0, 1.0, -1.0,
			1.0, -1.0, -1.0,
			-1.0, -1.0, -1.0
	};

	vector<GLfloat> vertexColors = {
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0,
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0
	};

	vector<GLubyte> indicies = { 0, 1, 2, 3, 4, 5, 6, 7 };

	//Mesh * cube = new Mesh(vertices,vertexColors,indicies);

}

void vertexColorShader(){
	renderEngine->shaderProgram->attachShader("tutorial4.vert", GL_VERTEX_SHADER);
	renderEngine->shaderProgram->attachShader("tutorial4.geom", GL_GEOMETRY_SHADER);
	renderEngine->shaderProgram->attachShader("tutorial4.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	renderEngine->shaderProgram->bindAttrib("in_Position");
	renderEngine->shaderProgram->bindAttrib("in_Color");
	renderEngine->shaderProgram->bindAttrib("in_Normal");
}

void brickShader(){
	renderEngine->shaderProgram->attachShader("brick.vert", GL_VERTEX_SHADER);
	renderEngine->shaderProgram->attachShader("brick.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	renderEngine->shaderProgram->bindAttrib("MCvertex");
	renderEngine->shaderProgram->bindAttrib(3,"MCnormal");

	//renderEngine->shaderProgram->bindAttrib("in_Color");
}

void initScene(){

	makeTetrahedron();
	//makeCube();
	vertexColorShader();
	//brickShader();



}

int main(int argc, char *argv[])
{

	mediaLayer = new MediaLayer(PROGRAM_NAME, 1024, 576);

	renderEngine = new RenderEngine();
	initScene();
	renderEngine->shaderProgram->linkAndUse();
	mediaLayer->renderLoop(renderEngine);

    delete renderEngine;
    delete mediaLayer;

}
