/*
 * Copyright © 2010 Lubosz Sarnecki
 * Textures.cpp
 *
 *  Created on: Oct 4, 2010
 */
#include <math.h>
#include <vector>
#include <string>
#include "Material/Textures.h"
#include "System/GUI.h"
#include "System/Config.h"
#include "System/Logger.h"
#include <QImage>
#include <QDebug>
#include "Renderer/RenderEngine.h"

DepthTexture::DepthTexture(QSize& res, string name) {
  glError;
  this->name = name;
  isDepth = true;
  glGenTextures(1, &handle);
  bind();
  LogDebug << "Creating FBO Depth texture #" << handle << " " << name;

  filterMinMag(GL_NEAREST, GL_NEAREST);

  // Specifies the texture comparison mode for currently bound depth textures.
  // That is, a texture whose internal format is GL_DEPTH_COMPONENT_*
  glTexParameteri(target, GL_TEXTURE_COMPARE_MODE,
          GL_COMPARE_REF_TO_TEXTURE
   );

  // Specifies the texture comparison mode for currently bound depth textures.
  // That is, a texture whose internal format is GL_DEPTH_COMPONENT_*
  glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE);

  // No need to force GL_DEPTH_COMPONENT24,
  // drivers usually give you the max precision if available
  glTexImage2D(target, 0, GL_DEPTH_COMPONENT32F, res.width(), res.height(), 0,
      GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  unbind();
  glError;
}

ShadowTexture::ShadowTexture(QSize& res, string name) {
  this->name = name;
  isDepth = true;
    glGenTextures(1, &handle);
    bind();
    LogDebug << "Creating FBO Shadow texture #" << handle << " " << name;

    /*
     GL_LINEAR does not make sense for depth texture.
     However, next tutorial shows usage of GL_LINEAR and PCF
     */
    filterMinMag(GL_LINEAR, GL_LINEAR);

    // Specifies the texture comparison mode for currently bound depth textures.
    // That is, a texture whose internal format is GL_DEPTH_COMPONENT_*
    glTexParameteri(target, GL_TEXTURE_COMPARE_MODE,
            GL_COMPARE_REF_TO_TEXTURE
     );

//    // Specifies the comparison operator used when GL_TEXTURE_COMPARE_MODE is
//    // set to GL_COMPARE_REF_TO_TEXTURE
//    glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//
//    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    // Remove artefact on the edges of the shadowmap
//     glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//     glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // No need to force GL_DEPTH_COMPONENT24,
    // drivers usually give you the max precision if available
    glTexImage2D(target, 0, GL_DEPTH_COMPONENT32F, res.width(), res.height(), 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    unbind();
  glError;
}

ColorTexture::ColorTexture(QSize& res, string name) {
  glError;
  this->name = name;

  glGenTextures(1, &handle);
  LogDebug << "Creating FBO Color texture #" << handle << " " << name;
  bind();

  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
  //            GL_COMPARE_REF_TO_TEXTURE);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);

  // FBO

  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  //
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


  filterMinMag(GL_NEAREST, GL_NEAREST);

  // Specifies the texture comparison mode for currently bound depth textures.
  // That is, a texture whose internal format is GL_DEPTH_COMPONENT_*
  glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE);

//  glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexImage2D(target, 0, GL_RGBA32F, res.width(), res.height(), 0, GL_RGBA, GL_FLOAT, 0);
  unbind();
  glError;
}

TextureFile::TextureFile(string filename, string name) {
  this->name = name;
  string path = Config::Instance().value<string> ("textureDir") + filename;
  glGenTextures(1, &handle);
  LogDebug << "Creating texture from file #" << handle << " " << name;
  bind();
  /*
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
   */
  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  loadFile(path);
  glGenerateMipmap(target);
  unbind();
}

TextureQImage::TextureQImage(QImage * image, string name) {
  this->name = name;
  glGenTextures(1, &handle);
  LogDebug << "Creating texture from qimage #" << handle << " " << name;
  bind();
  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  filterMinMag(GL_LINEAR, GL_LINEAR);
  loadQImage(image);
  unbind();
}

CubeTextureFile::CubeTextureFile(string filename, string name) {
  this->name = name;

  target = GL_TEXTURE_CUBE_MAP;

  glGenTextures(1, &handle);
  bind();

  filterMinMag(GL_LINEAR, GL_LINEAR);

  string textureDir = Config::Instance().value<string> ("textureDir");
  vector<string> suffixes = Config::Instance().values<string> ("suffixes");

  for (int face = 0; face < 6; face++) {
    //TODO: jpeg hardcoded
    string path = textureDir + filename + suffixes[face] + ".jpg";
    loadFile(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, path);
  }
}
