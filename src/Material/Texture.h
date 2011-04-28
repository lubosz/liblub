/*
 * Copyright © 2010 Lubosz Sarnecki
 * Texture.h
 *
 *  Created on: Mar 24, 2010
 */

#pragma once

#include "common/OpenGL.h"
#include <string>
#include <QImage>

using std::string;

class Texture {
 public:
	void activate();
	void bind();
	void uniform(GLuint program);
  GLuint getHandle() const;
	GLuint texture;
	string name;
 protected:
	GLenum glId;
	GLuint textureType;
	GLint glChannelOrder, texChannelOrder;

	virtual ~Texture();
public:
	void readQImage(GLenum target, string path);
	void loadQImage(GLenum target, QImage * image);
};
