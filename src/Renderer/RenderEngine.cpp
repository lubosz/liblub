/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 */
#include <vector>
#include "Scene/Camera.h"
#include "Scene/SceneData.h"
#include "Mesh/MeshLoader.h"
#include "Renderer/RenderEngine.h"
#include "System/Logger.h"
#include "System/Config.h"

RenderEngine::RenderEngine()
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

RenderEngine::~RenderEngine() {
    glError;

    /* Cleanup all the things we bound and allocated */
    LogInfo << "Shutting down Render Engine...";
    glError;
}

void RenderEngine::setClearColor(const QVector3D & backgroundColor){
  glClearColor(
          backgroundColor.x(), backgroundColor.y(), backgroundColor.z(),
          1.0);
}

void RenderEngine::toggleWire() {
  wire = !wire;
  setWire(wire);

}

void RenderEngine::setWire(bool wire){
  this->wire = wire;
  if (wire) {
    LogInfo << "Wireframe on";
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    LogInfo << "Wireframe off";
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

void RenderEngine::toggleFBO() {
  if (useFBO) {
    useFBO = false;
    LogInfo << "FBO Rendering diabled";
  } else {
    useFBO = true;
    LogInfo << "FBO Rendering enabled";
  }
}

void RenderEngine::toggleLightView() {
  if (lightView) {
    lightView = false;
  } else {
    lightView = true;
  }
}

void RenderEngine::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderEngine::checkVersion() {
    GLint maxTex1, maxTex2, MajorVersion, MinorVersion, numext, pointSize, uniformSize;
    LogInfo << "OpenGL" << glGetString(GL_VERSION);

    glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
    LogInfo << "Version" << MajorVersion << "." << MinorVersion;
    LogInfo << "GLSL" << glGetString(GL_SHADING_LANGUAGE_VERSION);

    glGetIntegerv(GL_POINT_SIZE, &pointSize);
    LogInfo << "Point Size" << pointSize;

    LogInfo << "Hardware"
            << glGetString(GL_VENDOR) << " - "
            << glGetString(GL_RENDERER);

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

