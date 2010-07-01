/*
 * Material.h
 *
 *  Created on: Mar 25, 2010
 *      Author: bmonkey
 */

#pragma once

#include <iostream>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "TextureFactory.h"

#include <boost/foreach.hpp>

using namespace std;

class Material {
public:
	ShaderProgram * shaderProgram;
	vector<Texture*> textures;

	virtual void uniforms() = 0;

	void init(){
		shaderProgram = new ShaderProgram();
	}

	void addTexture(Texture * texture){
		textures.push_back(texture);

	}

	void addTexture(string file,string name){
		textures.push_back(TextureFactory::Instance().load(file,name));

	}
	void addTextureCube(string file,string name){
		textures.push_back(TextureFactory::Instance().loadCubeMap(file,name));

	}

	void defaultAttribs(){

		shaderProgram->bindAttrib("in_Vertex");
		shaderProgram->bindAttrib("in_Color");
		shaderProgram->bindAttrib("in_Normal");
		shaderProgram->bindAttrib("in_Uv");

	}

	void attachVertFrag(string file){
		shaderProgram->attachShader(file + ".vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader(file + ".frag", GL_FRAGMENT_SHADER);
	}

	void attachVertFragGeom(string file){
		shaderProgram->attachShader(file + ".vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader(file + ".frag", GL_FRAGMENT_SHADER);
		shaderProgram->attachShader(file + ".geom", GL_FRAGMENT_SHADER);
	}

	void done(){
		shaderProgram->linkAndUse();
		defaultAttribs();
		initUniforms();
		glError("Material",68);
	}

	void initUniforms(){
		cout << "\nInitializing Material Uniforms for Shader #"<<shaderProgram->program<<"\n";
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
		glError("Material",84);
        BOOST_FOREACH( Texture* texture, textures ){
        	texture->activate();
        	texture->bind();
        	texture->uniform(shaderProgram->program);
        }

		uniforms();
		glError("Material",90);

	}

	void activateTextures(){
	    BOOST_FOREACH( Texture* texture, textures )
	    {
	    	texture->activate();
	    	texture->bind();

	    }
		glError("Material",102);
	}

	void activate(){

        //shaderProgram->use();
        activateTextures();
		glError("Material::activate",109);

	}
};


