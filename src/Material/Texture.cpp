/*
 * Copyright © 2010 Lubosz Sarnecki
 * Texture.cpp
 *
 *  Created on: Mar 24, 2010
 */
#include <string>
#include <QImage>
#include <QDebug>

#include "Material/Texture.h"
#include "System/Logger.h"
#include "Renderer/RenderEngine.h"

Texture::Texture()  {
  target = GL_TEXTURE_2D;
}

Texture::~Texture() {
  // TODO(bmonkey): Auto-generated destructor stub
}

GLuint Texture::getHandle() const {
    return texture;
}

void Texture::activate() {
    glActiveTexture(glId);
    glError;
}

void Texture::bind() {
  glBindTexture(target, texture);
    glError;
}

void Texture::uniform(GLuint program) {
    GLint texLoc = glGetUniformLocation(program, name.c_str());
    // TODO(bmonkey): -1 vs glId - GL_TEXTURE0
//    glUniform1i(texLoc, glId - GL_TEXTURE0);
    glUniform1i(texLoc, texture-1);
    glError;
}

void Texture::readQImage(GLenum target, string path) {
  QImage * image = new QImage();
  image->load(QString::fromStdString(path));

  //Qt loads image with wrong pixel order
  *image = image->mirrored(false, true);
//    QImage image = QGLWidget::convertToGLFormat(imageFoo);

  loadQImage(target, image);
}

void Texture::loadQImage(GLenum target, QImage * image) {
  if (image->bitPlaneCount() == 32) {
    glChannelOrder = GL_RGBA;
    texChannelOrder = GL_BGRA;
  } else {
    glChannelOrder = GL_RGB;
    //TODO: Qt needs A
    texChannelOrder = GL_BGRA;
  }
    glTexImage2D(target, 0, glChannelOrder, image->width(),
                 image->height(), 0, texChannelOrder, GL_UNSIGNED_BYTE,
                 image->bits());
//    glCopyTexImage2D(target, 0, glChannelOrder,
//    0, 0, 256, 256, 0);
//
//    glCopyTexImage2D(target, 1, glChannelOrder,
//    0, 0, 128, 128, 0);
//
//    glCopyTexImage2D(target, 2, glChannelOrder,
//    0, 0, 64, 64, 0);
    glError;

//    void glCopyTexImage2D(GLenum  target,  GLint  level,  GLenum  internalformat,  GLint  x,  GLint  y,  GLsizei  width,  GLsizei  height,  GLint  border);
//    void glTexImage2D(GLenum  target,  GLint  level,  GLint  internalFormat,  GLsizei  width,  GLsizei  height,  GLint  border,  GLenum  format,  GLenum  type,  const GLvoid *  data);
}
