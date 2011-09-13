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
    const GLchar *source = readFile(Config::Instance().value<string>("shaderDir") + fileName);

    /* Assign our handles a "name" to new shader objects */
    shader = glCreateShader(type);

    if (defines.size() > 0) {
      // set defines
      string defineString = "";

      foreach(string define, defines) {
        defineString += "#define " + define + "\n";
        LogDebug << "Shader Flags" << define;
      }
      const GLchar *sources[2] = { defineString.c_str(), source };
      glShaderSource(shader, 2, sources, NULL);
    } else {
      glShaderSource(shader, 1, &source, NULL);
    }
}

void Shader::loadTemplate() {
  string shaderSource = TemplateEngine::Instance().render(fileName).toStdString();
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

/* A simple function that will read a file
 * into an allocated char pointer buffer */
char* Shader::readFile(string filePath) {
    // Open
    std::ifstream f(filePath.c_str(), std::ios::binary);
    if (!f.is_open())
        LogFatal << "File not found:" << filePath;

    // Obtain file size
    f.seekg(0, std::ios::end); // Seek to end
    size_t size = f.tellg(); // Tell byte count
    f.seekg(0); // Seek back to start

    // Allocate buffer
    char* buffer = NULL;
    try {
        buffer = new char[size+1]; // +1 for terminating null
    } catch (...) {
        LogFatal << "Memory allocation failed when reading file:" << filePath;
    }

    // Read
    if (!f.read(buffer, size))
        LogFatal << "File reading error:" << filePath;

    buffer[size] = '\0'; // Null terminator

    return buffer; /* Return the buffer */
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
