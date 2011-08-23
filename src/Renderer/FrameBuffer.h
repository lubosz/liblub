/*
 * Copyright © 2010 Lubosz Sarnecki
 * FrameBuffer.h
 *
 *  Created on: Aug 31, 2010
 */

#pragma once

#include "Material/Texture.h"
#include "Material/Material.h"
#include "Mesh/Mesh.h"

class FrameBuffer {
 public:
	FrameBuffer(GLuint width, GLuint height);
	virtual ~FrameBuffer();
    void bind();
    void bindMulti();
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
	void checkFramebufferStatus();
};
