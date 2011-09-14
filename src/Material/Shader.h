/*
 * Copyright © 2010 Lubosz Sarnecki
 * Shader.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "common/OpenGL.h"
#include <vector>
#include <string>

using std::vector;
using std::string;
typedef char GLchar;
class Shader {
 public:
	Shader(string fileName, GLenum type, bool useTemplate);
	Shader(string fileName, GLenum type, const vector<string> & defines);
	GLuint getReference() const;
	virtual ~Shader();
	string shaderSource;
	string fileName;
 private:
	GLuint shader;

	GLenum type;

	vector<string> defines;

	string readFile(string filePath);
	void printShaderInfoLog(GLuint shader);
	void loadSource();
	void loadTemplate();
	void compile();
};

