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
#include "RenderSequence.h"

class RenderSequence;

class RenderEngine : public Singleton<RenderEngine> {
public:
	bool useFBO;
	void display();
	void toggleFBO();
	void clear();
	void toggleLightView();
	bool lightView;
	RenderSequence * shadowSequence;
private:
	friend class Singleton<RenderEngine>;

	unsigned frameCount;
	void checkVersion();
	GLboolean QueryExtension(char *extName);

	RenderEngine();
	virtual ~RenderEngine();

};
