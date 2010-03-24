/*
 * Texture.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include "Texture.h"

#include <FreeImagePlus.h>



Texture::Texture(string filename, GLenum glId) {
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->accessPixels());
    glActiveTexture(glId);
    glBindTexture(GL_TEXTURE_2D, texture);

}

Texture::~Texture() {
	// TODO Auto-generated destructor stub
}
