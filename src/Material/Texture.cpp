/*
 * Texture.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include "Texture.h"

Texture::~Texture() {
	// TODO Auto-generated destructor stub
}

GLuint Texture::getHandler() const
{
    return texture;
}

void Texture::activate(){
    glActiveTexture(glId);
    glError("Texture::activate",131);
}

void Texture::bind(){
	glBindTexture(textureType, texture);
    glError("Texture::bind",136);
}

void Texture::uniform(GLuint program){
	//cout << "Assigning Texture "<< "#"<< textureID << " " << name << " to program #"<< program << "\n";
	cout << "Assigning Texture "<< "#"<< " " << name << " to program #"<< program << "\n";
    GLint texLoc   = glGetUniformLocation(program, name.c_str());
    //TODO: -1?
    glUniform1i(texLoc, texture-1);
    glError("Texture::uniform",144);
}

fipImage * Texture::readImage(string path, GLint * glChannelOrder, GLint * texChannelOrder){

	fipImage * image = new fipImage();
	image->load( path.c_str());
    cout << "\n" << path << " Bits Per Pixel:\t" << image->getBitsPerPixel() << "\t" << image->getWidth() << "x" << image->getHeight() <<"\n";

    if (image->getBitsPerPixel() == 32) {
    	*glChannelOrder = GL_RGBA;
    	*texChannelOrder = GL_BGRA;
    }else if (image->getBitsPerPixel() == 24) {
    	*glChannelOrder = GL_RGB;
    	*texChannelOrder = GL_BGR;
    }else{
    	*glChannelOrder = GL_RGB;
    	*texChannelOrder = GL_BGR;
    	cout << "WARNING: Converting "<< path<< " to 24bits.\n";
    	if(image->convertTo24Bits()){
    		cout << "SUCESS!\n.";
    	}else{
    		cout << "ERROR: Converting "<< path<< " to 24bit failed.\n";
    	}
    }

    return image;
}
