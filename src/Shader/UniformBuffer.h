/*
 * UniformBuffer.h
 *
 *  Created on: Mar 23, 2011
 *      Author: bmonkey
 */

#pragma once

#include "Renderer/OpenGL.h"

class UniformBuffer {
public:
  UniformBuffer();
  virtual ~UniformBuffer();

  void bind();
  void alloc(GLuint size);
  void write(const void* data, GLint size);
  void writeSub(const void* data);
  void read(void* data);
  void copy(GLuint sourceBuffer);

  GLuint getHandle() {
    return handle;
  }
  GLint getSize() {
    return size;
  }

private:
  GLuint handle;
  GLint size;
  bool dynamic;
  GLenum target;
};

