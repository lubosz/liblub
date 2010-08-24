/*
 * config.h
 *
 *  Created on: Jun 8, 2010
 *      Author: bmonkey
 */
#include <string>

using namespace std;

#pragma once

const string shaderDir = "media/shaders/";
const string textureDir = "media/textures/";
const string meshDir = "media/meshes/";
static const string suffixes[6] = {"_RT", "_LF", "_DN","_UP", "_FR", "_BK"};
static const float backgroundColor[3] = {0.0,0.0,0.0};
const unsigned maxBuffers = 4;
#define DEBUG 0
#define USE_GL3
#define FPS_INTERVAL 1.0 //seconds.
#define VSync 0
