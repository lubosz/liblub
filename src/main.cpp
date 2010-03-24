/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include <FreeImagePlus.h>

#include "MediaLayer.h"
#include "RenderEngine.h"
#include "ShaderProgram.h"
#include "Mesh.h"

#define PROGRAM_NAME "Cone Step Mapping"

MediaLayer * mediaLayer;
RenderEngine * renderEngine;

void initTexture()
//void init2DTexture(GLint texName, GLint texWidth, GLint texHeight, GLubyte *texPtr)
{
	RenderEngine::Instance().glError("main.cpp",20);
	//GLint texName = "myTexture";

	fipImage *image = new fipImage();
	image->load("media/textures/bunny.png", PNG_DEFAULT);

    glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->accessPixels());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLint texLoc   = glGetUniformLocation(RenderEngine::Instance().shaderProgram->program, "myTexture");
    glUniform1i(texLoc, 0);
    RenderEngine::Instance().glError("main.cpp",38);

}


void makeTetrahedron(){
	vector<GLfloat> vertices = {
		    1.0,  1.0,  1.0,
		    -1.0, -1.0,  1.0,
		    -1.0,  1.0, -1.0,
		    1.0, -1.0, -1.0
	};

	vector<GLfloat> vertexColors = {
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0
	};

	vector<GLfloat> uvCoords = {
		1.0,  0.0,
		0.0,  1.0,
		0.0,  0.0,
		1.0,  1.0
	};

	vector<GLfloat> normals = {
		    1.0,  1.0,  1.0,
		    -1.0, -1.0,  1.0,
		    -1.0,  1.0, -1.0,
		    1.0, -1.0, -1.0
			/*
			0.4, -0.3, -0.8,
			0.4, -0.3, 0.8,
			-0.8, -0.5, 0.0,
			0.0, 0.9, 0.0
			*/
	};

	vector<GLubyte> indicies = { 0, 1, 2, 3, 0, 1 };

	Mesh * tetrahedron = new Mesh(vertices,vertexColors,normals,uvCoords,indicies);

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
	RenderEngine::Instance().shaderProgram->attachShader("tutorial4.vert", GL_VERTEX_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("tutorial4.geom", GL_GEOMETRY_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("tutorial4.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Normal");
}

void brickShader(){
	RenderEngine::Instance().shaderProgram->attachShader("brick.vert", GL_VERTEX_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("brick.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib(3,"MCnormal");
	//RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
}

void textureShader(){
	initTexture();

	RenderEngine::Instance().shaderProgram->attachShader("texture.vert", GL_VERTEX_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("texture.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Normal");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Uv");
}

void initScene(){

	RenderEngine::Instance().glError("main.cpp",150);
	makeTetrahedron();
	RenderEngine::Instance().glError("main.cpp",152);
	//makeCube();
	//vertexColorShader();
	//brickShader();
	textureShader();
	RenderEngine::Instance().glError("main.cpp",157);



}

int main(int argc, char *argv[])
{

	mediaLayer = new MediaLayer(PROGRAM_NAME, 1024, 576);
	RenderEngine::Instance().glError("main.cpp",164);
	//RenderEngine::RenderEngine();
	//RenderEngine::Instance().glError("main.cpp",164);
	//renderEngine = new RenderEngine();
	RenderEngine::Instance().glError("main.cpp",166);
	initScene();
	RenderEngine::Instance().glError("main.cpp",168);
	RenderEngine::Instance().shaderProgram->linkAndUse();

	RenderEngine::Instance().glError("main.cpp",171);

	GLint program = RenderEngine::Instance().shaderProgram->program;
	glUniform3f(glGetUniformLocation(program, "BrickColor"), 1.0, 0.3, 0.2);
	glUniform3f(glGetUniformLocation(program, "MortarColor"), 0.85, 0.86, 0.84);
	glUniform2f(glGetUniformLocation(program, "BrickSize"), 0.30, 0.15);
	glUniform2f(glGetUniformLocation(program, "BrickPct"), 0.90, 0.85);
	RenderEngine::Instance().glError("main.cpp",176);

	mediaLayer->renderLoop();
	RenderEngine::Instance().glError("main.cpp",179);
    //delete renderEngine;
    delete mediaLayer;

}
