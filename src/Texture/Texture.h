/*
 * Copyright © 2010 Lubosz Sarnecki
 * Texture.h
 *
 *  Created on: Mar 24, 2010
 */

#pragma once

#include "Renderer/OpenGL.h"
#include <string>
#include <QImage>

using std::string;

class Texture {
 public:
  Texture(string name);
	void activate();
	void bind();
	void unbind();
	void uniform(GLuint program);
    string checkName(string name);
  GLuint getHandle() const;
	virtual ~Texture();
	string name;
	bool isDepth;
    string path;
    GLint glChannelOrder, texChannelOrder;
 protected:
	GLuint handle;
	GLenum glId;
	GLuint target;

public:
	void loadFile(const string & path);
    void loadQImage(const QImage &image);
  void loadFile(GLenum target, const string & path);
  void loadQImage(GLenum target, const QImage &image);
	void filterMinMag(GLenum min, GLenum mag);
};
