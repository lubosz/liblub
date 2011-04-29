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
    glError;
}

void Texture::filterMinMag(GLenum min, GLenum mag) {
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
}
