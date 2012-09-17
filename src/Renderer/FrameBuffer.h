/*
 * Copyright © 2010 Lubosz Sarnecki
 * FrameBuffer.h
 *
 *  Created on: Aug 31, 2010
 */

#pragma once

#include "Shader/ShaderProgram.h"
#include "Mesh/Mesh.h"

class FrameBuffer {
 public:
	FrameBuffer(QSize& res);
	virtual ~FrameBuffer();
    void bind();
    void setDrawBuffers(GLsizei count);
    void setDrawBuffers(vector<GLenum>& buffers);
    void setDrawBuffer(GLenum buffer);
    void unBind();
    void updateRenderView();

	QSize res;

	void check();
	void attachTexture(Texture * texture);
	void attachTextures(vector<Texture*> &textures);
	void disableColorBuffer();
 private:
	GLuint handle;
	GLuint renderBufferHandle;
	GLuint attachmentPoint;

	void printFramebufferInfo();
    string getTextureParameters(GLuint id);
    string getRenderbufferParameters(GLuint id);
    string convertInternalFormatToString(GLint format);
	void checkFramebufferStatus();
};
