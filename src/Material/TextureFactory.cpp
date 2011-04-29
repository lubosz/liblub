/*
 * Copyright © 2010 Lubosz Sarnecki
 * TextureFactory.cpp
 *
 *  Created on: Mar 25, 2010
 */
#include "Material/TextureFactory.h"
#include "System/Logger.h"

TextureFactory::TextureFactory() {
  textureCount = 0;
}

TextureFactory::~TextureFactory() {
  // TODO(bmonkey): Auto-generated destructor stub
}

GLenum TextureFactory::getNextId() {
  if (textureCount > 31) {
    LogFatal << "No more Texture slots available.";
  }
  GLenum nextId = textureEnums[textureCount];
  textureCount++;
  return nextId;
}
