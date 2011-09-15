/*
 * Copyright © 2010 Lubosz Sarnecki
 * Material.cpp
 *
 *  Created on: Aug 30, 2010
 */
#include <string>
#include "Scene/SceneGraph.h"
#include "System/Logger.h"
#include "Renderer/RenderEngine.h"
#include "Material/Textures.h"

void Material::init() {
    shaderProgram = new ShaderProgram();
    textures = vector<Texture*>();
}

void Material::addTexture(Texture * texture) {
    textures.push_back(texture);
}

void Material::addTextures(vector<Texture *> &addTextures) {
  foreach(Texture * texture, addTextures)
    textures.push_back(texture);
}

void Material::addTexture(string file, string name) {
    textures.push_back(new TextureFile(file, name));
}
void Material::addTextureCube(string file, string name) {
    textures.push_back(new CubeTextureFile(file, name));
}

void Material::activateAndBindTextures() {
    foreach(Texture* texture, textures) {
            texture->activate();
            texture->bind();
        }
    glError;
}

void Material::activateTextures() {
    foreach(Texture* texture, textures) {
            texture->activate();
        }
    glError;
}

void Material::bindTextures() {
    foreach(Texture* texture, textures) {
            texture->bind();
        }
    glError;
}

void Material::initRenderTargets(vector<Texture*> &targets){
  GLuint program = shaderProgram->getHandle();
  for(unsigned i = 0; i < targets.size(); i++){
    if(!targets[i]->isDepth)
      glBindFragDataLocation(program, i, targets[i]->name.c_str());
  }
}

void Material::samplerUniforms() {
    foreach(Texture* texture, textures) {
            texture->uniform(shaderProgram->getHandle());
        }
}

ShaderProgram *Material::getShaderProgram() {
    return shaderProgram;
}
