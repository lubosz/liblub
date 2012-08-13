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

#include "Scene/Scene.h"

Texture::Texture(string name) : name(name), isDepth(false), target(GL_TEXTURE_2D) {
  glId = TextureFactory::Instance().getNextId();
  Scene::Instance().textures.insert(checkName(name), this);
}

string Texture::checkName(string name) {
    if (!Scene::Instance().textures.contains(name)) {
        return name;
    } else {
        LogWarning << "Texture name" << name << "already taken.";
        return checkName(name+"1");
    }
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
//    LogDebug << name << ": Setting Sampler Uniform in Program" << program << "Handle: "<< handle-1;
    glError;
}

void Texture::loadFile(const string & path) {
  loadFile(target, path);
}

void Texture::loadFile(GLenum target, const string & path) {
    if (path == "")
        LogError << "Path is empty.";

    this->path = path;
  QImage image = QImage();
  image.load(QString::fromStdString(path));

  if(image.isNull())
    image.load(QString::fromStdString(Config::Instance().value<string> ("textureDir") + path));
  if(image.isNull())
    LogError << path << "does not exist";

  //TODO: Qt loads image with wrong pixel order
  image = image.mirrored(false, true);
  loadQImage(target, image);
}

void Texture::loadQImage(const QImage & image) {
  loadQImage(target, image);
}

void Texture::loadQImage(GLenum target, const QImage& image) {
    QImage img = image;

    texChannelOrder = GL_BGRA;

    switch(img.bitPlaneCount()) {
        case 32:
            glChannelOrder = GL_RGBA;
            break;
        case 24:
            glChannelOrder = GL_RGB;
            break;
        default:
            if (img.hasAlphaChannel()) {
                img = img.convertToFormat(QImage::Format_ARGB32);
                glChannelOrder = GL_RGBA;
            } else {
                img = img.convertToFormat(QImage::Format_RGB32);
                glChannelOrder = GL_RGB;
            }
            break;
    }

    glTexImage2D(target, 0, glChannelOrder, img.width(),
                 img.height(), 0, texChannelOrder, GL_UNSIGNED_BYTE,
                 img.bits());
    glError;
}

void Texture::filterMinMag(GLenum min, GLenum mag) {
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
}
