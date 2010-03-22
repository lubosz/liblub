/*
 * ShaderSystem.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once


#include <string>
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include "Shader.h"

using namespace std;

class ShaderProgram {
public:

	/* These pointers will receive the contents of our shader source code files */
	//GLchar *vertexsource, *fragmentsource, *geometrysource;

	/* These are handles used to reference the shaders */
	//GLuint vertexshader, fragmentshader, geometryshader;

	/* This is a handle to the shader program */
	GLuint program;

	//string shaderDir;

	ShaderProgram();
	virtual ~ShaderProgram();

	void printProgramInfoLog();

	void attachShader(Shader *shader);
};
