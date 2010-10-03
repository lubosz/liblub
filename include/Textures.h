/*
 * Textures.h
 *
 *  Created on: Oct 4, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Texture.h"
#include <math.h>

class DepthTexture : public Texture {
public:
	DepthTexture(GLuint width, GLuint height, string name, GLenum glId){
		this->name = name;
		this->glId = glId;
		textureType = GL_TEXTURE_2D;

	    glGenTextures(1, &texture);
	    cout << "Creating FBO texture #" << texture << " " << name << "\n";
	    glBindTexture(textureType, texture);

	    //shadowmap
		// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
		glTexImage2D( textureType, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	    //Shadowmap
		// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Remove artefact on the edges of the shadowmap
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	    glBindTexture(textureType, 0);

  }
};


class ColorTexture : public Texture {
public:
	ColorTexture(GLuint width, GLuint height, string name, GLenum glId){
		this->name = name;
		this->glId = glId;
		textureType = GL_TEXTURE_2D;

	    glGenTextures(1, &texture);
	    cout << "Creating FBO texture #" << texture << " " << name << "\n";
	    glBindTexture(textureType, texture);
	    glTexImage2D(textureType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	/*
	    //glTexParameterf(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    //glTexParameterf(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	    glTexParameterf(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameterf(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    //glTexParameterf(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    //glTexParameterf(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	    //glGenerateMipmap(textureType);
	*/


	    //GOOD

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  }
};

class TextureFile : public Texture {
public:
	TextureFile(string filename, GLenum glId, string name){
		textureType = GL_TEXTURE_2D;

		//this->textureID = textureID;
		this->glId = glId;

		this->name = name;

		string path = textureDir + filename;
		GLint * glChannelOrder = new GLint();
		GLint * texChannelOrder = new GLint();
	    fipImage * image = readImage(path, glChannelOrder, texChannelOrder);

		glGenTextures(1, &texture);
		cout << "Creating texture #" << texture << " " << name << "\n";

	    glBindTexture(GL_TEXTURE_2D, texture);
	/*
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	*/
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	    glTexImage2D(GL_TEXTURE_2D, 0, *glChannelOrder, image->getWidth(), image->getHeight(), 0, *texChannelOrder, GL_UNSIGNED_BYTE, image->accessPixels());

  }
};

class SplatTexture : public Texture {
public:
	SplatTexture(GLenum glId, string name, int resolution){
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

	/**
	* EvalHermite(float pA, float pB, float vA, float vB, float u)
	* @brief Evaluates Hermite basis functions for the specified coefficients.
	*/
	inline float evalHermite(float pA, float pB, float vA, float vB, float u)
	{
	    float u2=(u*u), u3=u2*u;
	    float B0 = 2*u3 - 3*u2 + 1;
	    float B1 = -2*u3 + 3*u2;
	    float B2 = u3 - 2*u2 + u;
	    float B3 = u3 - u;
	    return( B0*pA + B1*pB + B2*vA + B3*vB );
	}

	unsigned char* createGaussianMap(int N)
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

};


class CubeTextureFile : public Texture {
public:
	CubeTextureFile(string filename, GLenum glId, string name, bool cube) {
		this->glId = glId;
		this->name = name;

		textureType = GL_TEXTURE_CUBE_MAP;

		glGenTextures(1, &texture);
		glBindTexture(textureType,texture);

		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


		for(int face=0; face<6; face++) {
			string path = textureDir + filename + suffixes[face] + ".jpg";

			GLint * glChannelOrder = new GLint();
			GLint * texChannelOrder = new GLint();
			fipImage * image = readImage(path, glChannelOrder, texChannelOrder);

		    glBindTexture(textureType, texture);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, *glChannelOrder, image->getWidth(), image->getHeight(), 0, *texChannelOrder, GL_UNSIGNED_BYTE, image->accessPixels());
		}

	}
};





