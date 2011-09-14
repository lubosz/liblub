/*
 * Copyright © 2010 Lubosz Sarnecki
 * Shader.cpp
 *
 *  Created on: Mar 22, 2010
 */

#include "Material/Shader.h"

#include <QtCore>
#include <string>
#include <vector>
#include <fstream>
#include "System/Logger.h"
#include "System/Config.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderEngine.h"

#include <fstream>
#include <streambuf>

using std::ifstream;
using std::istreambuf_iterator;

Shader::Shader(string fileName, GLenum type, bool useTemplate) {
  LogDebug << "Creating Shader" << fileName;
  this->fileName = fileName;
  this->type = type;

  if (useTemplate)
    loadTemplate();
  else
    loadSource();
  compile();
}

Shader::Shader(string fileName, GLenum type, const vector<string> & defines) {
  LogDebug << "Creating Shader" << fileName;
  this->fileName = fileName;
  this->type = type;
  this->defines = defines;

  loadSource();
  compile();
}

Shader::~Shader() {
  glDeleteShader(shader);
}


void Shader::loadSource() {
  /* Read our shaders into the appropriate buffers */
    shaderSource = readFile(Config::Instance().value<string>("shaderDir") + fileName);

    /* Assign our handles a "name" to new shader objects */
    shader = glCreateShader(type);

    if (defines.size() > 0) {
      // set defines
      string defineString = "";

      foreach(string define, defines) {
        defineString += "#define " + define + "\n";
        LogDebug << "Shader Flags" << define;
      }
      const GLchar *sources[2] = { defineString.c_str(), shaderSource.c_str() };
      glShaderSource(shader, 2, sources, NULL);
    } else {
        const GLchar *source = shaderSource.c_str();
      glShaderSource(shader, 1, &source, NULL);
    }
}

void Shader::loadTemplate() {
  shaderSource = TemplateEngine::Instance().render(fileName).toStdString();
//  printf("%s:\n\n %s\n", fileName.c_str(), shaderSource.c_str());
  /* Assign our handles a "name" to new shader objects */
  shader = glCreateShader(type);
  /* Set rendered template string as source */
  const GLchar *source = shaderSource.c_str();
  glShaderSource(shader, 1,  &source, NULL);
}

void Shader::compile() {
  LogDebug << "Compiling Shader#" << shader << "...";
  /* Compile our shader objects */
  // TODO(bmonkey): driver crashes :/
  glCompileShader(shader);
  printShaderInfoLog(shader);
  glError;
}

string Shader::readFile(string filePath) {
    string source;
    try {
        ifstream t(filePath);
        source = string((istreambuf_iterator<char> (t)),
                istreambuf_iterator<char> ());
    } catch (...) {
        LogFatal << "Could Not Load File:" << filePath;
    }

    if(source == ""){
        LogFatal << "Empty Shader file" << filePath;
    }

    return source;
}

void Shader::printShaderInfoLog(GLuint shader) {
  int infologLen = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
  if (infologLen > 1) {
    GLchar * infoLog = reinterpret_cast<GLchar*>(malloc(infologLen));
    if (infoLog == NULL) {
      LogError << "Could not allocate InfoLog buffer";
    }
    int charsWritten = 0;
    glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
    string shaderlog = infoLog;
    free(infoLog);
    LogError << "Shader Log"<< shaderlog;
  } else {
    LogDebug << "Success";
  }
}

GLuint Shader::getReference() const {
  return shader;
}
