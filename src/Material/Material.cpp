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

void Material::defaultAttribs() {

	shaderProgram->bindAttrib("in_Vertex");
	//TODO: Buffer order not variable
	//shaderProgram->bindAttrib("in_Color");
	shaderProgram->bindAttrib("in_Normal");
	shaderProgram->bindAttrib("in_Tangent");
	shaderProgram->bindAttrib("in_Bitangent");
	shaderProgram->bindAttrib("in_Uv");

}



void Material::done() {
	defaultAttribs();
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
	glError("Material", 84);
	foreach( Texture* texture, textures )
				{
					//TODO: Why is this not needed?
					//texture->activate();
					texture->bind();
					texture->uniform(shaderProgram->getReference());
				}

	uniforms();
    SceneGraph::Instance().light->bindShaderInit(shaderProgram);
	glError("Material", 90);

}

void Material::activateTextures() {
	foreach( Texture* texture, textures )
				{
					texture->activate();
					texture->bind();

				}
	glError("Material::activateTextures", 102);
}

void Material::activate() {

	//shaderProgram->use();
	activateTextures();
	glError("Material::activate", 109);

}

ShaderProgram *Material::getShaderProgram() const {
	return shaderProgram;
}
