/*
 * Copyright © 2010 Lubosz Sarnecki
 * Texture.cpp
 *
 *  Created on: Mar 24, 2010
 */
#include <string>
#include "Material/Texture.h"

Texture::~Texture() {
	// TODO(bmonkey): Auto-generated destructor stub
}

GLuint Texture::getHandler() const {
    return texture;
}

void Texture::activate() {
    glActiveTexture(glId);
    glError("Texture::activate", 131);
}

void Texture::bind() {
	glBindTexture(textureType, texture);
    glError("Texture::bind", 136);
}

void Texture::uniform(GLuint program) {
    GLint texLoc = glGetUniformLocation(program, name.c_str());
    // TODO(bmonkey): -1 vs glId - GL_TEXTURE0
//    glUniform1i(texLoc, glId - GL_TEXTURE0);
    glUniform1i(texLoc, texture-1);

    glError("Texture::uniform", 144);
}

fipImage * Texture::readImage(
        string path, GLint * glChannelOrder, GLint * texChannelOrder) {
	fipImage * image = new fipImage();
	image->load(path.c_str());
	Logger::Instance().message
	        << path
	        << " Bits Per Pixel:\t"
	        << image->getBitsPerPixel() << "\t"
	        << image->getWidth()
	        << "x" << image->getHeight();
    Logger::Instance().log("DEBUG", "Texture");

    if (image->getBitsPerPixel() == 32) {
    	*glChannelOrder = GL_RGBA;
    	*texChannelOrder = GL_BGRA;
    } else if (image->getBitsPerPixel() == 24) {
    	*glChannelOrder = GL_RGB;
    	*texChannelOrder = GL_BGR;
    } else {
    	*glChannelOrder = GL_RGB;
    	*texChannelOrder = GL_BGR;
    	Logger::Instance().log("WARNING",
    	        "Texture", "Converting "+ path+ " to 24bits.");
    	if (image->convertTo24Bits()) {
    		Logger::Instance().log("WARNING", "Texture", "SUCESS!");
    	} else {
    		Logger::Instance().log("ERROR",
    		        "Texture", "Converting "+ path+ " to 24bit failed.");
    	}
    }

    return image;
}
