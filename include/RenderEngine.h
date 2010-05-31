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

class RenderEngine {
public:

	static RenderEngine& Instance()
	 {
	    static RenderEngine singleton;
	    return singleton;
	  }

	ShaderProgram * shaderProgram;
	SceneGraph * sceneGraph;

	unsigned frameCount;

	//RenderEngine();
	//virtual ~RenderEngine();

	void glError(string file, int line);
	void checkVersion();
	GLboolean QueryExtension(char *extName);
	void display();

private:
	RenderEngine();                                 // Private constructor
	~RenderEngine();
  RenderEngine(const RenderEngine&);                 // Prevent copy-construction
  RenderEngine& operator=(const RenderEngine&);      // Prevent assignment



};
