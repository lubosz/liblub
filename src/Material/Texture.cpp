/*
 * Texture.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include "Texture.h"


#include <iostream>
#include <math.h>

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

//Splat texture
Texture::Texture(GLenum glId, string name, int resolution) {
    unsigned char* data = createGaussianMap(resolution);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
#ifndef USE_GL3
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
#else
    glTexParameteri(GL_TEXTURE_2D, 0, GL_TRUE);
#endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, resolution, resolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

Texture::Texture(string filename, GLenum glId, string name, bool cube) {
	this->glId = glId;
	this->name = name;

	textureType = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP,texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


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

//------------------------------------------------------------------------------
// Function     	  : EvalHermite
// Description	    :
//------------------------------------------------------------------------------
/**
* EvalHermite(float pA, float pB, float vA, float vB, float u)
* @brief Evaluates Hermite basis functions for the specified coefficients.
*/
inline float Texture::evalHermite(float pA, float pB, float vA, float vB, float u)
{
    float u2=(u*u), u3=u2*u;
    float B0 = 2*u3 - 3*u2 + 1;
    float B1 = -2*u3 + 3*u2;
    float B2 = u3 - 2*u2 + u;
    float B3 = u3 - u;
    return( B0*pA + B1*pB + B2*vA + B3*vB );
}

unsigned char* Texture::createGaussianMap(int N)
{
    float *M = new float[2*N*N];
    unsigned char *B = new unsigned char[4*N*N];
    float X,Y,Y2,Dist;
    float Incr = 2.0f/N;
    int i=0;
    int j = 0;
    Y = -1.0f;
    //float mmax = 0;
    for (int y=0; y<N; y++, Y+=Incr)
    {
        Y2=Y*Y;
        X = -1.0f;
        for (int x=0; x<N; x++, X+=Incr, i+=2, j+=4)
        {
            Dist = (float)sqrtf(X*X+Y2);
            if (Dist>1) Dist=1;
            M[i+1] = M[i] = evalHermite(1.0f,0,0,0,Dist);
            B[j+3] = B[j+2] = B[j+1] = B[j] = (unsigned char)(M[i] * 255);
        }
    }
    delete [] M;
    return(B);
}

void Texture::activate(){
    glActiveTexture(glId);
}

void Texture::bind(){
	glBindTexture(textureType, texture);
}

void Texture::uniform(GLuint program){
	cout << "Assigning Texture "<< "#"<< texture << " " << name << " to program #"<< program << "\n";
    GLint texLoc   = glGetUniformLocation(program, name.c_str());
    //glUniform1i(texLoc, texture-1);
    glUniform1i(texLoc, texture);
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
