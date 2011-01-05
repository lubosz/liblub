/*
 * Copyright © 2010 Lubosz Sarnecki
 * OpenGL.h
 *
 *  Created on: Oct 13, 2010
 */

#pragma once

#include "common.h"

#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include <GL3/glext.h>

#include "Logger.h"

static void glError(string file, int line) {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                error = "Unknown error";
                break;
		}

		Logger::Instance().message << "GL_" << error << " - " << file << ':' << line;
	    Logger::Instance().log("ERROR", "glError");
		exit(0);
	}
}
