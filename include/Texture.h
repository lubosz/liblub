/*
 * Texture.h
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#pragma once

#include "OpenGL.h"

#include <FreeImagePlus.h>

class Texture {
 public:
	void activate();
	void bind();
	void uniform(GLuint program);
    GLuint getHandler() const;
	string name;
 protected:
	GLenum glId;
	GLuint texture;
	GLuint textureType;

	virtual ~Texture();

	fipImage * readImage(
	        string path, GLint * glChannelOrder, GLint * texChannelOrder
	);
};
