/*
 * UniformBuffer.cpp
 *
 *  Created on: Mar 23, 2011
 *      Author: bmonkey
 */

#include "Shader/UniformBuffer.h"

UniformBuffer::UniformBuffer() {
  target = GL_UNIFORM_BUFFER;
  glGenBuffers(1, &handle);
  dynamic = false;
}

UniformBuffer::~UniformBuffer() {
  glDeleteBuffers(1, &handle);
}

void UniformBuffer::bind() {
  glBindBuffer(target, handle);
}

void UniformBuffer::alloc(GLuint size) {
  this->size = size;
  //data will be passed later
  //static draw if not modified
  glBufferData(target, size, NULL, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void UniformBuffer::write(const void* data, GLuint size) {
  this->size = size;
  glBufferData(target, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void UniformBuffer::writeSub(const void* data) {
  glBufferSubData(target, 0, size, data);
}

void UniformBuffer::read(void* data) {
  glGetBufferSubData(target, 0, size, data);
}

void UniformBuffer::copy(GLuint sourceBuffer) {
  //read target
  glBindBuffer(target, sourceBuffer);
  //write target
  glBindBuffer(GL_COPY_WRITE_BUFFER, handle);

  glCopyBufferSubData(target, GL_COPY_WRITE_BUFFER, 0, 0, size);
}
