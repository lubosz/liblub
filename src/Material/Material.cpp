/*
 * Material.cpp
 *
 *  Created on: Aug 30, 2010
 *      Author: bmonkey
 */
#include "Material.h"
#include "SceneGraph.h"
#include <boost/foreach.hpp>

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

void Material::attachVertFrag(string file) {
	shaderProgram->attachShader(file + ".vert", GL_VERTEX_SHADER);
	shaderProgram->attachShader(file + ".frag", GL_FRAGMENT_SHADER);
}
void Material::attachVertFrag(string file, const vector<string> & defines){
	shaderProgram->attachShader(file + ".vert", GL_VERTEX_SHADER, defines);
	shaderProgram->attachShader(file + ".frag", GL_FRAGMENT_SHADER, defines);
}

void Material::attachVertGeom(string file) {
	shaderProgram->attachShader(file + ".vert", GL_VERTEX_SHADER);
	shaderProgram->attachShader(file + ".geom", GL_GEOMETRY_SHADER);
}

void Material::attachVertFragGeom(string file) {
	shaderProgram->attachShader(file + ".vert", GL_VERTEX_SHADER);
	shaderProgram->attachShader(file + ".geom", GL_GEOMETRY_SHADER);
	shaderProgram->attachShader(file + ".frag", GL_FRAGMENT_SHADER);
}

void Material::done() {
	defaultAttribs();
	shaderProgram->linkAndUse();

	initUniforms();
	glError("Material", 68);
}

void Material::initUniforms() {
	cout << "\nInitializing Material Uniforms for Shader #"
			<< shaderProgram->getReference() << "\n";
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
	BOOST_FOREACH( Texture* texture, textures )
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
	BOOST_FOREACH( Texture* texture, textures )
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
