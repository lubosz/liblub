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

#ifdef LIBLUB_WINDOWS
# include <GL/wglew.h>
# define glfGetProcAddress wglGetProcAddress
# include "WinGL.h"
#else
# define GLCOREARB_PROTOTYPES 1
# define GL_GLEXT_PROTOTYPES 1
# include <GL/glcorearb.h>
# include <GL/glext.h>
#endif

#include "Common/Singleton.h"
#include <System/Logger.h>
#define glError OpenGL::checkGlError(__FILE__,__LINE__)

class QVector3D;
class QSize;

class OpenGL : public Singleton<OpenGL> {
    friend class Singleton<OpenGL>;
public:
    bool useFBO;
    bool lightView;
    bool wire;
    void toggleFBO();
    void toggleWire();
    void setWire(bool wire);
    void clear();
    void toggleLightView();
    void setClearColor(const QVector3D & backgroundColor);
    void updateViewport(QSize& size);
    static void checkGlError(const char* file, int line);
    static void checkVersion();
    void setContextCreated(bool contextCreated);
    bool getContextCreated (){
        return isContextCreated;
    }
 private:

    bool isContextCreated;

    unsigned frameCount;
    GLboolean QueryExtension(char *extName);

    OpenGL();
    virtual ~OpenGL();

};
