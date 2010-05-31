/*
 * ShaderSystem.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once


#include <string>
#include <list>

/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include "Shader.h"

using namespace std;

class ShaderProgram {
public:

	/* This is a handle to the shader program */
	GLuint program;
	list<Shader*> shaders;
	unsigned attribCount;

	ShaderProgram();
	virtual ~ShaderProgram();

	void printProgramInfoLog();

	void attachShader(string fileName, GLenum type);
	void detachShader(Shader *shader);

	void bindAttrib(string name);
	void bindAttrib(unsigned position, string name);

	void linkAndUse();

	void setNormalMatrix(GLfloat * modelmatrix);
	void setModelViewMatrix(GLfloat * mvMatrix);
	void setModelViewProjectionMatrix(GLfloat * mvpMatrix);
	void setLightPosition(GLfloat x, GLfloat y, GLfloat z);
};
