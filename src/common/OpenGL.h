/*
 * Copyright © 2010 Lubosz Sarnecki
 * OpenGL.h
 *
 *  Created on: Oct 13, 2010
 */

#pragma once

#ifdef USE_GLEW
# include <GL/glew.h>
#endif

#ifdef WIN32
//#define GL3_PROTOTYPES 1
//# include <GL3/gl3.h>
//# include <wingdi.h>
//# include <GL/gl.h>
//# include <GL/glext.h>
# include <GL/wglew.h>
//#include <QtOpenGL>
# define glfGetProcAddress wglGetProcAddress
#include "WinGL.h"
#else
#define GL3_PROTOTYPES 1
# define GL_GLEXT_PROTOTYPES 1
# include <GL3/gl3.h>
# include <GL3/glext.h>
#endif



#include <System/Logger.h>
#define glError OpenGL::checkGlError(__FILE__,__LINE__)

class OpenGL {
public:
  static void checkGlError(const char* file, int line) {
//    glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");

    GLenum err(glGetError());

    while (err != GL_NO_ERROR) {
      string error;

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
      Logger(file, line, Logger::Fatal) << "GL_" << error;
    }
  }
};
