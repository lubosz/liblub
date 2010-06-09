/*
 * Texture.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include "Texture.h"


#include <iostream>

Texture::Texture(string filename, GLenum glId, string name) {
	textureType = GL_TEXTURE_2D;

	this->glId = glId;
	this->name = name;

	string path = textureDir + filename;
	GLint * glChannelOrder = new GLint();
	GLint * texChannelOrder = new GLint();
    fipImage * image = readImage(path, glChannelOrder, texChannelOrder);

	glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, *glChannelOrder, image->getWidth(), image->getHeight(), 0, *texChannelOrder, GL_UNSIGNED_BYTE, image->accessPixels());
}


Texture::Texture(string filename, GLenum glId, string name, bool cube) {
	this->glId = glId;
	this->name = name;

	textureType = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &texture);

/*
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
*/



	for(int face=0; face<6; face++) {
		string path = textureDir + filename + suffixes[face] + ".jpg";

		GLint * glChannelOrder = new GLint();
		GLint * texChannelOrder = new GLint();
		fipImage * image = readImage(path, glChannelOrder, texChannelOrder);

	    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, *glChannelOrder, image->getWidth(), image->getHeight(), 0, *texChannelOrder, GL_UNSIGNED_BYTE, image->accessPixels());
	}

}

Texture::~Texture() {
	// TODO Auto-generated destructor stub
}

void Texture::activate(){
    glActiveTexture(glId);
    //glBindTexture(GL_TEXTURE_2D, texture);

    if (textureType == GL_TEXTURE_CUBE_MAP){
        glEnable(GL_TEXTURE_CUBE_MAP);
    }

    glBindTexture(textureType, texture);

    if (textureType == GL_TEXTURE_CUBE_MAP){
        glEnable(GL_TEXTURE_WRAP_S);
        glEnable(GL_TEXTURE_WRAP_T);
        glEnable(GL_TEXTURE_WRAP_R);
    }

/*
    glTexGeni(GL_S, GL_TEXTURE_, GL_);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
*/
}

void Texture::uniform(GLuint program){
    GLint texLoc   = glGetUniformLocation(program, name.c_str());
    glUniform1i(texLoc, texture-1);
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
