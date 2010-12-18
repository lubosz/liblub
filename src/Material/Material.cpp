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
	shaderProgram->defaultAttribs();
	shaderProgram->linkAndUse();

	initUniforms();
	glError("Material", 68);
}

void Material::initUniforms() {
	Logger::Instance().message << "Initializing Material Uniforms for Shader #" << shaderProgram->getReference();
    Logger::Instance().log("DEBUG","Material");

	/*
	 GLint texLoc;
	 for (int i = 0; i < textures.size(); i++){
	 cout << "Assigning Texture " << i << " " << textures[i]->name << "\n";
	 textures[i]->activate();
	 textures[i]->bind();
	 texLoc   = glGetUniformLocation(shaderProgram->program, textures[i]->name.c_str());
	 glUniform1i(texLoc, i);
	 }
	 */
	bindTextures();
	shaderProgram->initUniforms();
	uniforms();
	glError("Material", 54);

}

void Material::activateTextures() {
	foreach( Texture* texture, textures )
				{
					texture->activate();
					texture->bind();

				}
	glError("Material::activateTextures", 102);
}

void Material::bindTextures(){
	foreach( Texture* texture, textures )
				{
					//TODO: Why is this not needed?
					//texture->activate();
					texture->bind();
					texture->uniform(shaderProgram->getReference());
				}
}

void Material::activate() {

	//shaderProgram->use();
	activateTextures();
	glError("Material::activate", 109);

}

ShaderProgram *Material::getShaderProgram() {
	return shaderProgram;
}
