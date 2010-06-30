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
		cout << "Creating program #" << shaderProgram->program <<"\n";
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
	}

	void initUniforms(){
        BOOST_FOREACH( Texture* texture, textures ){
        	texture->activate();
        	texture->bind();
        }

		uniforms();

	}

	void activateTextures(){
	    BOOST_FOREACH( Texture* texture, textures )
	    {
	    	texture->activate();
	    	texture->bind();

	    }
	}

	void activate(){
        shaderProgram->use();
        activateTextures();
	}
};


