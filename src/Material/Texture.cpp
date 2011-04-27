/*
 * Copyright © 2010 Lubosz Sarnecki
 * Texture.cpp
 *
 *  Created on: Mar 24, 2010
 */
#include <string>

#if USE_FREEIMAGE
#include <FreeImagePlus.h>
#else
#include <QImage>
#include <QDebug>
#endif

#include "Material/Texture.h"
#include "System/Logger.h"
#include "Renderer/RenderEngine.h"

Texture::~Texture() {
  // TODO(bmonkey): Auto-generated destructor stub
}

GLuint Texture::getHandler() const {
    return texture;
}

void Texture::activate() {
    glActiveTexture(glId);
    glError;
}

void Texture::bind() {
  glBindTexture(textureType, texture);
    glError;
}

void Texture::uniform(GLuint program) {
    GLint texLoc = glGetUniformLocation(program, name.c_str());
    // TODO(bmonkey): -1 vs glId - GL_TEXTURE0
//    glUniform1i(texLoc, glId - GL_TEXTURE0);
    glUniform1i(texLoc, texture-1);
    glError;
}
#if USE_FREEIMAGE
void Texture::readFreeImage(GLenum target, string path) {
  fipImage * image = new fipImage();
  image->load(path.c_str());
  Logger::Instance().message
          << path
          << " Bits Per Pixel:\t"
          << image->getBitsPerPixel() << "\t"
          << image->getWidth()
          << "x" << image->getHeight();
    Logger::Instance().log("DEBUG", "Texture");

    if (image->getBitsPerPixel() == 32) {
      glChannelOrder = GL_RGBA;
      texChannelOrder = GL_BGRA;
    } else if (image->getBitsPerPixel() == 24) {
      glChannelOrder = GL_RGB;
      texChannelOrder = GL_BGR;
    } else {
      glChannelOrder = GL_RGB;
      texChannelOrder = GL_BGR;
      Logger::Instance().log("WARNING",
              "Texture", "Converting "+ path+ " to 24bits.");
      if (image->convertTo24Bits()) {
        Logger::Instance().log("WARNING", "Texture", "SUCESS!");
      } else {
        Logger::Instance().log("ERROR",
                "Texture", "Converting "+ path+ " to 24bit failed.");
      }
    }

    glTexImage2D(target, 0, glChannelOrder, image->getWidth(),
                image->getHeight(), 0, texChannelOrder, GL_UNSIGNED_BYTE,
                image->accessPixels());
}
#else
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
#endif
