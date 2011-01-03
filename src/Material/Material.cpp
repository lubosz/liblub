/*
 * Material.cpp
 *
 *  Created on: Aug 30, 2010
 *      Author: bmonkey
 */
#include "SceneGraph.h"

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

void Material::done() {
	//todo: deprecated

	shaderProgram->init();
    uniforms();
    samplerUniforms();

	glError("Material", 68);
}

void Material::activateTextures() {
	foreach( Texture* texture, textures )
				{
					texture->activate();
					texture->bind();

				}
	glError("Material::activateTextures", 102);
}

void Material::samplerUniforms(){
	foreach( Texture* texture, textures )
				{
					texture->uniform(shaderProgram->getReference());
				}
}

void Material::activate() {
	//todo: check performance
	//rebinding textures for shared programs
	samplerUniforms();
	activateTextures();
	glError("Material::activate", 109);

}

ShaderProgram *Material::getShaderProgram() {
	return shaderProgram;
}
