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

    cout << "\n\nImage getBitsPerPixel\n\n" << image->getBitsPerPixel();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->accessPixels());
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
