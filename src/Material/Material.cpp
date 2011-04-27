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

void Material::init() {
    diffuseColor = QVector4D();
    shaderProgram = new ShaderProgram();
}

void Material::addTexture(Texture * texture) {
    textures.push_back(texture);
}

void Material::addTexture(string file, string name) {
    textures.push_back(TextureFactory::Instance().load(file, name));
}
void Material::addTextureCube(string file, string name) {
    textures.push_back(TextureFactory::Instance().loadCubeMap(file, name));
}

void Material::done(const QList<string> & attributes) {
    // TODO(bmonkey): deprecated, materials defined in xml
    shaderProgram->init(attributes);
    uniforms();
    samplerUniforms();

    glError;
}

void Material::activateTextures() {
    foreach(Texture* texture, textures) {
            texture->activate();
            texture->bind();
        }
    glError;
}

void Material::samplerUniforms() {
    foreach(Texture* texture, textures) {
            texture->uniform(shaderProgram->getReference());
        }
}

void Material::activate() {
    // TODO(bmonkey): check performance (rebinding textures for shared programs)
    samplerUniforms();
    activateTextures();
    glError;
}

ShaderProgram *Material::getShaderProgram() {
    return shaderProgram;
}
