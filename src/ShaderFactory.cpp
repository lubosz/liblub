/*
 * ShaderFactory.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include <FreeImagePlus.h>
#include "ShaderFactory.h"
#include "Texture.h"
#include "RenderEngine.h"

ShaderFactory::ShaderFactory() {
	// TODO Auto-generated constructor stub

}

ShaderFactory::~ShaderFactory() {
	// TODO Auto-generated destructor stub
}

void ShaderFactory::vertexColorShader(){
	RenderEngine::Instance().shaderProgram->attachShader("tutorial4.vert", GL_VERTEX_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("tutorial4.geom", GL_GEOMETRY_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("tutorial4.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Normal");
}

void ShaderFactory::brickShader(){
	RenderEngine::Instance().shaderProgram->attachShader("brick.vert", GL_VERTEX_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("brick.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib(3,"MCnormal");
	//RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
}

void ShaderFactory::textureShader(){

	Texture * bunny = new Texture("cone/collage_step.png",GL_TEXTURE0);

	RenderEngine::Instance().shaderProgram->attachShader("texture.vert", GL_VERTEX_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("texture.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Normal");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Uv");
}

void ShaderFactory::multiTextureShader(){

	//Texture * bunny = new Texture("bunny.png",GL_TEXTURE0);


	RenderEngine::Instance().shaderProgram->attachShader("multitexture.vert", GL_VERTEX_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("multitexture.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Normal");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Uv");
}

void ShaderFactory::earthShader(){

	Texture("earth/day.jpg",GL_TEXTURE0);
	Texture("earth/night.jpg",GL_TEXTURE1);
	Texture("earth/clouds.jpg",GL_TEXTURE2);

	RenderEngine::Instance().shaderProgram->attachShader("eath.vert", GL_VERTEX_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("eath.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Normal");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Uv");
}

void ShaderFactory::coneMapShader(){

	Texture("cone/collage_base.jpg",GL_TEXTURE0);
	//Texture("cone/collage_height.jpg",GL_TEXTURE1);
	Texture("cone/collage_step.png",GL_TEXTURE1);

	RenderEngine::Instance().shaderProgram->attachShader("shared.vert", GL_VERTEX_SHADER);
	//RenderEngine::Instance().shaderProgram->attachShader("cone.frag", GL_FRAGMENT_SHADER);
	RenderEngine::Instance().shaderProgram->attachShader("csm_dist_shadow.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Position");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Color");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Normal");
	RenderEngine::Instance().shaderProgram->bindAttrib("in_Uv");
}

void ShaderFactory::addUniforms(){

	GLuint texture, otherTexture;
	fipImage *image = new fipImage();
    fipImage *otherImage = new fipImage();

    string textureDir = "media/textures/";
	string path = textureDir + "cone/collage_base.jpg";

	image->load( path.c_str());
    path = textureDir + "cone/collage_step.png";
    otherImage->load( path.c_str());

	glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->accessPixels());



    glGenTextures(1, &otherTexture);

    glBindTexture(GL_TEXTURE_2D, otherTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, otherImage->getWidth(), otherImage->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, otherImage->accessPixels());



    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, otherTexture);


	GLint program = RenderEngine::Instance().shaderProgram->program;


    GLint texLoc   = glGetUniformLocation(program, "myTexture");
    glUniform1i(texLoc, 0);

    texLoc   = glGetUniformLocation(program, "myOtherTexture");
    glUniform1i(texLoc, 1);

    cout << "Texture:" << texture << "\n";
    cout << "Other Texture:" << otherTexture << "\n";

	/*
	glUniform3f(glGetUniformLocation(program, "BrickColor"), 1.0, 0.3, 0.2);
	glUniform3f(glGetUniformLocation(program, "MortarColor"), 0.85, 0.86, 0.84);
	glUniform2f(glGetUniformLocation(program, "BrickSize"), 0.30, 0.15);
	glUniform2f(glGetUniformLocation(program, "BrickPct"), 0.90, 0.85);
	 */

	/*
    GLint texLoc   = glGetUniformLocation(program, "EarthDay");
    glUniform1i(texLoc, 0);
    texLoc   = glGetUniformLocation(program, "EarthNight");
    glUniform1i(texLoc, 1);
    texLoc   = glGetUniformLocation(program, "EarthCloudGloss");
    glUniform1i(texLoc, 2);
    */
/*
	glUniform4f(glGetUniformLocation(program, "ambient"), 1.0, 1.0, 1.0, 1.0);
	glUniform4f(glGetUniformLocation(program, "diffuse"), 0.1, 0.3, 0.9, 1.0);

	glUniform1f(glGetUniformLocation(program, "depth"), 1.5);
	glUniform1f(glGetUniformLocation(program, "texsize"), 1.5);
	glUniform1f(glGetUniformLocation(program, "csm_gain"), 1.5);
	glUniform1f(glGetUniformLocation(program, "csm_offset"), 1.5);
	glUniform1f(glGetUniformLocation(program, "linearAttenuation"), 1.5);

	glUniform1i(glGetUniformLocation(program, "conesteps"), 3);

    GLint texLoc   = glGetUniformLocation(program, "texmap");
    glUniform1i(texLoc, 0);
    //texLoc   = glGetUniformLocation(program, "heightmap");
    //glUniform1i(texLoc, 1);
    texLoc   = glGetUniformLocation(program, "stepmap");
    glUniform1i(texLoc, 1);
    */
}
