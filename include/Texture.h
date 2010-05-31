/*
 * Texture.h
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
using namespace std;

class Texture {
public:
	GLenum glId;
	string name;
	GLuint texture;
	Texture(string filename, GLenum glId, string name);
	virtual ~Texture();
	void activate();
	void uniform(GLuint program);
};
