#include "config.h"

#pragma once

//#define USE_GL3

#ifdef USE_GL3
#define GL3_PROTOTYPES 1
#define LIBLUB_GL_MAJOR_VERION 3
#define LIBLUB_GL_MINOR_VERION 3
#ifndef GLfloat
#include <GL3/gl3.h>
#include <GL3/glext.h>
//#include "gl3.h"
#endif
#else
#define GL_GLEXT_PROTOTYPES 1
#define LIBLUB_GL_MAJOR_VERION 2
#define LIBLUB_GL_MINOR_VERION 1
//#include <GL/glew.h>
#include <GL/gl.h>
//#include <GL3/glu.h>
#include <GL/glext.h>
#endif



#include <iostream>
using namespace std;
