/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 */
#include <vector>
#include "Scene/Camera.h"
#include "Mesh/MeshFactory.h"
#include "Renderer/RenderEngine.h"
#include "System/Logger.h"
#include "System/Config.h"

RenderEngine::RenderEngine()
:
    useFBO(false), lightView(false), wire(false), frameCount(0) {
    glError;
    checkVersion();
    shadowSequence = new RenderSequence();

    /* Make our background black */
    vector<float> backgroundColor =
            Config::Instance().values<float> ("backgroundColor");
    glClearColor(
            backgroundColor[0], backgroundColor[1], backgroundColor[2],
            1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);

    glPointSize(5);

    // glBindSampler();
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_POINT_SPRITE_ARB);
    // glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    // glEnable(GL_POLYGON_OFFSET_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     glEnable(GL_BLEND);
//          glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
//          glBlendColor(1,1,1,0);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//     glBlendEquation(GL_MAX);
    // glDepthMask(GL_FALSE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glError;
}

RenderEngine::~RenderEngine() {
    glError;

    /* Cleanup all the things we bound and allocated */
    Logger::Instance().message << "Shutting down Render Engine...";
    Logger::Instance().log("MESSAGE");
    glError;
}

void RenderEngine::toggleWire() {
    wire = !wire;
    if (wire) {
        Logger::Instance().log("MESSAGE", "Wireframe", "on");
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        Logger::Instance().log("MESSAGE", "Wireframe", "off");
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void RenderEngine::toggleFBO() {
    if (useFBO) {
        Logger::Instance().message << "FBO Rendering diabled";
        Logger::Instance().log("MESSAGE");
        useFBO = false;
    } else {
        useFBO = true;
        Logger::Instance().message << "FBO Rendering enabled";
        Logger::Instance().log("MESSAGE");
    }
}

void RenderEngine::display() {
    /*
     * Uniform Animation
     *
     SceneGraph::Instance().transform(frameCount);
     GLfloat floatanim = 10.0/GLfloat(frameCount%100);
     glUniform4f(glGetUniformLocation(shaderProgram->program, "ScaleFactor"), floatanim, floatanim, floatanim, floatanim);
     glUniform2f(glGetUniformLocation(shaderProgram->program, "Offset"), floatanim, floatanim);
     int mode = int(frameCount/100.0)%10;
     glUniform1i(glGetUniformLocation(shaderProgram->program, "Mode"), mode);
     cout << "Mode:\t" << mode << "\n";
     frameCount++;
     */

    shadowSequence->render();
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
    GLint maxTex1, maxTex2, MajorVersion, MinorVersion, numext, pointSize;

    Logger::Instance().message << glGetString(GL_VERSION);
    Logger::Instance().log("MESSAGE", "OpenGL");

    glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
    Logger::Instance().message << MajorVersion << "." << MinorVersion;
    Logger::Instance().log("MESSAGE", "Version");

    Logger::Instance().message << glGetString(GL_SHADING_LANGUAGE_VERSION);
    Logger::Instance().log("MESSAGE", "GLSL");

    glGetIntegerv(GL_POINT_SIZE, &pointSize);
    Logger::Instance().message << pointSize;
    Logger::Instance().log("MESSAGE", "Point Size");

    Logger::Instance().message << glGetString(GL_VENDOR) << " - "
            << glGetString(GL_RENDERER);
    Logger::Instance().log("MESSAGE", "Hardware");

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxTex1);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTex2);
    Logger::Instance().message << maxTex1 << " " << maxTex2;
    Logger::Instance().log("MESSAGE", "MaxTex");

    glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
    Logger::Instance().message << "Found " << numext << " GL_EXTENSIONS";
    Logger::Instance().log("MESSAGE", "GL_EXTENSIONS");

    // for (int i = 0; i < numext; i++) {
    //  Logger::Instance().message << glGetStringi(GL_EXTENSIONS,i);
    //  Logger::Instance().log("DEBUG","GL_EXTENSIONS");
    // }
}



