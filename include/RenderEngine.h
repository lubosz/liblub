/*
 * RenderEngine.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include <string>
#include <iostream>

#include "ShaderProgram.h"

using namespace std;

class RenderEngine {
public:

	static RenderEngine& Instance()
	 {
	    static RenderEngine singleton;
	    return singleton;
	  }

	ShaderProgram * shaderProgram;

	GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */
	GLfloat modelmatrix[16]; /* Our model matrix  */

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
