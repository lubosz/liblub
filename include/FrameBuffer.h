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
	void toggle();
private:
	GLuint fboId;                       // ID of FBO
	GLuint rboId;                       // ID of Renderbuffer object
	Material * pass1Mat;
	Material * pass2Mat;
	//Texture * fboTexture;
	unsigned width, height;

	Mesh * renderPlane;
	bool useFBO;

	void printFramebufferInfo();
	void bindShaders(ShaderProgram * shaderProgram);
	string getTextureParameters(GLuint id);
	string getRenderbufferParameters(GLuint id);
	string convertInternalFormatToString(GLenum format);
	bool checkFramebufferStatus();
};
