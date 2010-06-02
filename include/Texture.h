/*
 * Texture.h
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"

#include <string>
#include <FreeImagePlus.h>

const string textureDir = "media/textures/";
static const string suffixes[6] = {"_RT", "_LF", "_UP", "_DN", "_FR", "_BK"};

class Texture {
public:
	GLenum glId;
	string name;
	GLuint texture;
	GLuint textureType;
	Texture(string filename, GLenum glId, string name);
	Texture(string filename, GLenum glId, string name, bool cube);
	virtual ~Texture();
	void activate();
	void uniform(GLuint program);
	fipImage * readImage(string path, GLint * glChannelOrder, GLint * texChannelOrder);
};
