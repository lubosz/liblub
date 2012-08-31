/*
 * Copyright © 2010 Lubosz Sarnecki
 * Shader.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "Renderer/OpenGL.h"
#include <vector>
#include <string>

using std::vector;
using std::string;
typedef char GLchar;
class Shader {
 public:
    Shader(const string &fileName, GLenum type);
	GLuint getHandle() const;
	virtual ~Shader();
    string shaderSource;
    string fileName;
 private:
	GLuint handle;
	GLenum type;

	vector<string> defines;

	string readFile(string filePath);
	void printShaderInfoLog(GLuint shader);
	void loadTemplate();
	void compile();
};

