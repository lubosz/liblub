/*
 * Texture.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include "Texture.h"

#include <FreeImagePlus.h>
#include <iostream>

Texture::Texture(string filename, GLenum glId, string name) {
	this->glId = glId;
	this->name = name;
	fipImage *image = new fipImage();
	string textureDir = "media/textures/";
	string path = textureDir + filename;
	image->load( path.c_str());

	glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    cout << "\n" << filename << " Bits Per Pixel:\t" << image->getBitsPerPixel() <<"\t"<< image->getWidth()<< "x" <<image->getHeight() <<"\n";

    GLint type, internal;

    if (image->getBitsPerPixel() == 32) {
    	internal = GL_RGBA;
    	type = GL_BGRA;
    }else if (image->getBitsPerPixel() == 24) {
    	internal = GL_RGB;
    	type = GL_BGR;
    }else{
    	internal = GL_RGB;
    	type = GL_BGR;
    	cout << "WARNING: Converting "<< filename<< " to 24bits.\n";
    	if(image->convertTo24Bits()){
    		cout << "SUCESS!\n.";
    	}else{
    		cout << "ERROR: Converting "<< filename<< " to 24bit failed.\n";
    	}
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internal, image->getWidth(), image->getHeight(), 0, type, GL_UNSIGNED_BYTE, image->accessPixels());
}

Texture::~Texture() {
	// TODO Auto-generated destructor stub
}

void Texture::activate(){
    glActiveTexture(glId);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::uniform(GLuint program){
    GLint texLoc   = glGetUniformLocation(program, name.c_str());
    glUniform1i(texLoc, texture-1);
}
