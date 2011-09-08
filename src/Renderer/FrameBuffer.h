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
	FrameBuffer(QSize& res);
	virtual ~FrameBuffer();
    void bind();
    void setDrawBuffers(unsigned count);
    void setDrawBuffers(vector<GLenum>& buffers);
    void setDrawBuffer(GLenum buffer);
    void unBind();
    void draw(Material * material);
    void updateRenderView();

	QSize res;

	void check();
	void attachTexture(Texture * texture);
	void attachTextures(vector<Texture*> &textures);
	void disableColorBuffer();
 private:
	GLuint fboId;                       // ID of FBO
	GLuint rboId;                       // ID of Renderbuffer object
	GLuint attachmentPoint;

	Mesh * renderPlane;

	void printFramebufferInfo();
	string getTextureParameters(GLuint id);
	string getRenderbufferParameters(GLuint id);
	string convertInternalFormatToString(GLenum format);
	void checkFramebufferStatus();
};
