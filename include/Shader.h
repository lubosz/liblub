/*
 * Shader.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

using namespace std;

const string shaderDir = "media/shaders/";

class Shader {
public:
	/* These pointers will receive the contents of our shader source code files */
	GLchar *source;

	/* These are handles used to reference the shaders */
	GLuint shader;


	string fileName;
	GLenum type;

	Shader(string fileName, GLenum type);
	virtual ~Shader();

	char* readFile(string file);
	void printShaderInfoLog(GLuint shader);
	void reload();
	void loadAndCompile();
};

