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
  Texture();
	void activate();
	void bind();
	void unbind();
	void uniform(GLuint program);
  GLuint getHandle() const;
	virtual ~Texture();
	string name;
 protected:
	GLuint handle;
	GLenum glId;
	GLuint target;
	GLint glChannelOrder, texChannelOrder;

public:
	void loadFile(const string & path);
	void loadQImage(QImage * image);
  void loadFile(GLenum target, const string & path);
  void loadQImage(GLenum target, QImage * image);
	void filterMinMag(GLenum min, GLenum mag);
};
