/*
 * Shader.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include "common.h"

using namespace std;



class Shader {
public:
	Shader(string fileName, GLenum type);
	GLuint getReference() const;
	virtual ~Shader();
	void reload();
private:
	/* These pointers will receive the contents of our shader source code files */
	GLchar *source;

	/* These are handles used to reference the shaders */
	GLuint shader;

	GLenum type;
	string fileName;

	char* readFile(string file);
	void printShaderInfoLog(GLuint shader);

	void loadAndCompile();
};

