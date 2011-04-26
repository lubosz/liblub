/*
 * UniformBuffer.h
 *
 *  Created on: Mar 23, 2011
 *      Author: bmonkey
 */

#pragma once

#include "common/OpenGL.h"

class UniformBuffer {
public:
  UniformBuffer();
  virtual ~UniformBuffer();

  void bind();
  void alloc(GLuint size);
  void write(const void* data, GLuint size);
  void writeSub(const void* data);
  void read(void* data);
  void copy(GLuint sourceBuffer);

  GLuint getHandle() {
    return handle;
  }
  GLuint getSize() {
    return size;
  }

private:
  GLuint handle;
  GLuint size;
  bool dynamic;
  GLenum target;
};

