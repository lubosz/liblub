/*
 * FrameBuffer.h
 *
 *  Created on: Aug 31, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class FrameBuffer {
public:
	FrameBuffer(GLuint width, GLuint height);
	virtual ~FrameBuffer();
    void bind();
    void unBind();
    void draw(Material * material);
    void updateRenderView();

	unsigned width, height;

	void checkAndFinish();
	void attachTexture(GLenum attachmentPoint, Texture * texture);
	void disableColorBuffer();
private:
	GLuint fboId;                       // ID of FBO
	GLuint rboId;                       // ID of Renderbuffer object

	Mesh * renderPlane;

	void printFramebufferInfo();
	string getTextureParameters(GLuint id);
	string getRenderbufferParameters(GLuint id);
	string convertInternalFormatToString(GLenum format);
	string checkFramebufferStatus();
};
