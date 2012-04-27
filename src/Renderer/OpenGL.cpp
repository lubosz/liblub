/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 */
#include <vector>
#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include "Mesh/MeshLoader.h"
#include "Renderer/OpenGL.h"
#include "System/Logger.h"
#include "System/Config.h"

namespace {
    // Behaves nicely if glGetString returns null
    std::string glGetStringSafe(GLenum name) {
        const char* str = reinterpret_cast<const char*>(glGetString(name));
        if (!str) return "null";
        else return str;
    }
}


OpenGL::OpenGL()
:
    useFBO(false), lightView(false), wire(false), frameCount(0) {
    glError;
    checkVersion();

    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);

//    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
//    glMatrixMode(GL_PROJECTION);
//    glPointSize(5);

    // glBindSampler();
//     glEnable(GL_CULL_FACE);
//     glEnable(GL_POINT_SPRITE_ARB);
//     glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    // glEnable(GL_POLYGON_OFFSET_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//          glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
//          glBlendColor(1,1,1,0);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//     glBlendEquation(GL_MAX);
    // glDepthMask(GL_FALSE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//     glDepthRangef(0.0,1.0);

    glError;
}

OpenGL::~OpenGL() {
    glError;

    /* Cleanup all the things we bound and allocated */
    LogInfo << "Shutting down Render Engine...";
    glError;
}

void OpenGL::setClearColor(const QVector3D & backgroundColor){
  glClearColor(
          backgroundColor.x(), backgroundColor.y(), backgroundColor.z(),
          1.0);
}

void OpenGL::toggleWire() {
  wire = !wire;
  setWire(wire);

}

void OpenGL::setWire(bool wire){
  this->wire = wire;
  if (wire) {
    LogInfo << "Wireframe on";
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    LogInfo << "Wireframe off";
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

void OpenGL::toggleFBO() {
  if (useFBO) {
    useFBO = false;
    LogInfo << "FBO Rendering diabled";
  } else {
    useFBO = true;
    LogInfo << "FBO Rendering enabled";
  }
}

void OpenGL::toggleLightView() {
  if (lightView) {
    lightView = false;
  } else {
    lightView = true;
  }
}

void OpenGL::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL::checkVersion() {
    GLint maxTex1, maxTex2, MajorVersion, MinorVersion, numext, pointSize, uniformSize;

    string version = glGetStringSafe(GL_VERSION);

    if (version == "null") {
        LogError << "OpenGL is uninitialized";
    }

    LogInfo << "OpenGL" << version;

    glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);

    LogInfo << "Version" << MajorVersion << "." << MinorVersion;

    LogInfo << "GLSL" << glGetStringSafe(GL_SHADING_LANGUAGE_VERSION);

    glGetIntegerv(GL_POINT_SIZE, &pointSize);
    LogInfo << "Point Size" << pointSize;

    LogInfo << "Hardware"
            << glGetStringSafe(GL_VENDOR) << " - "
            << glGetStringSafe(GL_RENDERER);

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxTex1);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTex2);
    LogInfo << "MaxTex" << maxTex1 << " " << maxTex2;

    glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
    LogInfo << "Found " << numext << " GL_EXTENSIONS";

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &uniformSize);
    LogInfo << "GL_MAX_VERTEX_UNIFORM_BLOCKS" << uniformSize;

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &uniformSize);
    LogInfo << "GL_MAX_FRAGMENT_UNIFORM_BLOCKS" << uniformSize;

    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &uniformSize);
    LogInfo << "GL_MAX_GEOMETRY_UNIFORM_BLOCKS" << uniformSize;

    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &uniformSize);
    LogInfo << "GL_MAX_UNIFORM_BLOCK_SIZE" << uniformSize;

    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    LogInfo << "Max Number of Color Buffer Attachment Points: "
            << colorBufferCount;

//    GLfloat floatvalue;
//    glGetFloatv(GL_DEPTH_BIAS, &floatvalue);
//    LogInfo << floatvalue;
//    Logger::Instance().log("MESSAGE", "GL_DEPTH_BIAS");

    // for (int i = 0; i < numext; i++) {
    //  LogInfo << glGetStringi(GL_EXTENSIONS,i);
    //  Logger::Instance().log("DEBUG","GL_EXTENSIONS");
    // }
    glError;
}

void OpenGL::updateViewport(QSize& size){
  glViewport(0, 0, size.width(), size.height());
}

void OpenGL::checkGlError(const char* file, int line) {
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

void OpenGL::setContextCreated(bool contextCreated) {
    isContextCreated = true;
}
