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

class RenderEngine : public Singleton<RenderEngine> {
public:

	friend class Singleton<RenderEngine>;

	//ShaderProgram * shaderProgram;
	unsigned frameCount;

	void checkVersion();
	GLboolean QueryExtension(char *extName);
	void display();

private:
	RenderEngine();
	virtual ~RenderEngine();

};
