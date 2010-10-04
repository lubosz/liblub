/*
 * FrameBuffer.h
 *
 *  Created on: Aug 31, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class FrameBuffer {
public:
	FrameBuffer(GLuint width, GLuint height);
	virtual ~FrameBuffer();
    void bind();
    void unBind();
    void draw();
    void updateRenderView();

	unsigned width, height;

	void checkAndFinish();
	void attachTexture(GLenum attachmentPoint, Texture * texture);
	void disableColorBuffer();
	Texture * getDebugTexture();
private:
	GLuint fboId;                       // ID of FBO
	GLuint rboId;                       // ID of Renderbuffer object

	Mesh * renderPlane;
	Material * debugMat;

	void printFramebufferInfo();
	string getTextureParameters(GLuint id);
	string getRenderbufferParameters(GLuint id);
	string convertInternalFormatToString(GLenum format);
	bool checkFramebufferStatus();
};
