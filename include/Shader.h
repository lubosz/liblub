/*
 * Shader.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include "OpenGL.h"

class Shader {
public:
	Shader(string fileName, GLenum type);
	Shader(string fileName, GLenum type, const vector<string> & defines);
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

	vector<string> defines;

	char* readFile(string filePath);
	void printShaderInfoLog(GLuint shader);

	void loadAndCompile();
};

