/*
 * FrameBuffer.h
 *
 *  Created on: Aug 31, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"

	const int SCREEN_WIDTH = 400;
	const int SCREEN_HEIGHT = 300;
	const int TEXTURE_WIDTH = 256;
	const int TEXTURE_HEIGHT = 256;

class FrameBuffer {
public:
	FrameBuffer();
	virtual ~FrameBuffer();
private:
	GLuint fboId;                       // ID of FBO
	GLuint textureId;                   // ID of texture
	GLuint rboId;                       // ID of Renderbuffer object



	void printFramebufferInfo();
	string getTextureParameters(GLuint id);
	string getRenderbufferParameters(GLuint id);
	string convertInternalFormatToString(GLenum format);
};
