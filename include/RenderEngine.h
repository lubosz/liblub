/*
 * RenderEngine.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include <string>
#include <iostream>
#include <string.h>
#include "ShaderProgram.h"
#include "SceneGraph.h"
#include "Singleton.h"
#include "FrameBuffer.h"

class RenderEngine : public Singleton<RenderEngine> {
public:
	void display();
private:
	friend class Singleton<RenderEngine>;

	//ShaderProgram * shaderProgram;
	unsigned frameCount;
	FrameBuffer *fbo;
	Mesh * renderPlane;

	void checkVersion();
	GLboolean QueryExtension(char *extName);

	void clear();
	RenderEngine();
	virtual ~RenderEngine();

};
