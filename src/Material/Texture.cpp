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
#include "Material/TextureFactory.h"
#include "System/Logger.h"
#include "Renderer/OpenGL.h"
#include "System/Config.h"

Texture::Texture()  {
  isDepth = false;
  target = GL_TEXTURE_2D;
  glId = TextureFactory::Instance().getNextId();
}

Texture::~Texture() {
  // TODO(bmonkey): Auto-generated destructor stub
}

GLuint Texture::getHandle() const {
    return handle;
}

void Texture::activate() {
    glActiveTexture(glId);
}

void Texture::bind() {
  glBindTexture(target, handle);
}

void Texture::unbind() {
  glBindTexture(target, 0);
}


void Texture::uniform(GLuint program) {
    GLint texLoc = glGetUniformLocation(program, name.c_str());
    // TODO(bmonkey): -1 vs glId - GL_TEXTURE0
//    glUniform1i(texLoc, glId - GL_TEXTURE0);
//    glUniform1i(texLoc, handle-1);
    glUniform1i(texLoc, handle-1);
    LogDebug << name << ": Setting Sampler Uniform in Program" << program << "Handle: "<< handle-1;
    glError;
}

void Texture::loadFile(const string & path) {
  loadFile(target, path);
}

void Texture::loadFile(GLenum target, const string & path) {
    if (path == "")
        LogError << "Path is empty.";

  QImage * image = new QImage();
  image->load(QString::fromStdString(path));

  if(image->isNull())
    image->load(QString::fromStdString(Config::Instance().value<string> ("textureDir") + path));
  if(image->isNull())
    LogError << path << "does not exist";

  //TODO: Qt loads image with wrong pixel order
  *image = image->mirrored(false, true);
  loadQImage(target, image);
  delete image;
}

void Texture::loadQImage(QImage * image) {
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
