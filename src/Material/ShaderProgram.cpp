/*
 * Copyright © 2010 Lubosz Sarnecki
 * ShaderSystem.cpp
 *
 *  Created on: Mar 22, 2010
 */
#include <assert.h>
#include <vector>
#include <string>
#include "Material/ShaderProgram.h"
#include "Scene/SceneGraph.h"
#include "System/Logger.h"
#include "Scene/SceneData.h"

ShaderProgram::ShaderProgram() {
  attribCount = 0;
    /* Assign our program handle a "name" */
    program = glCreateProgram();
    Logger::Instance().message << "Creating Program #" << program;
    Logger::Instance().log("DEBUG", "ShaderProgram");
}

ShaderProgram::~ShaderProgram() {
    Logger::Instance().log("MESSAGE", "ShaderProgram",
            "Shutting down Shader System...");
    glUseProgram(0);

    while (shaders.size() > 0) {
        detachShader(shaders.back());
    }
}

void ShaderProgram::printProgramInfoLog() {
  int infologLen = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLen);
  if (infologLen > 1) {
    GLchar * infoLog = reinterpret_cast<GLchar*>(malloc(infologLen));
    if (infoLog == NULL) {
      Logger::Instance().log("ERROR", "Program Log",
              "Could not allocate InfoLog buffer");
    }
    int charsWritten = 0;
    glGetProgramInfoLog(program, infologLen, &charsWritten, infoLog);
    string shaderlog = infoLog;
    free(infoLog);
    Logger::Instance().log("ERROR", "Program Log", shaderlog);
  } else {
    Logger::Instance().log("DEBUG", "Program", "Done");
  }
}

void ShaderProgram::attachShader(string fileName, GLenum type) {
    /* Attach our shaders to our program */
  Shader * shader = new Shader(fileName, type);
  shaders.push_back(shader);
  glAttachShader(program, shader->getReference());
}

void ShaderProgram::attachShader(
        string fileName, GLenum type, const vector<string> & defines) {
    /* Attach our shaders to our program */
  Shader * shader = new Shader(fileName, type, defines);
  shaders.push_back(shader);
  glAttachShader(program, shader->getReference());
}

void ShaderProgram::detachShader(Shader *shader) {
  glDetachShader(program, shader->getReference());
//  shaders.remove(shader);
  delete shader;
}

void ShaderProgram::attachVertFrag(string file) {
  attachShader(file + ".vert", GL_VERTEX_SHADER);
  attachShader(file + ".frag", GL_FRAGMENT_SHADER);
}
void ShaderProgram::attachVertFrag(
        string file, const vector<string> & defines) {
    attachShader(file + ".vert", GL_VERTEX_SHADER, defines);
    attachShader(file + ".frag", GL_FRAGMENT_SHADER, defines);
}

void ShaderProgram::attachVertGeom(string file) {
  attachShader(file + ".vert", GL_VERTEX_SHADER);
  attachShader(file + ".geom", GL_GEOMETRY_SHADER);
}

void ShaderProgram::attachVertFragGeom(string file) {
  attachShader(file + ".vert", GL_VERTEX_SHADER);
  attachShader(file + ".geom", GL_GEOMETRY_SHADER);
  attachShader(file + ".frag", GL_FRAGMENT_SHADER);
}

void ShaderProgram::bindAttrib(unsigned position, string name) {
  glBindAttribLocation(program, position, name.c_str());
}

void ShaderProgram::bindAttribIfUnbound(string name) {
  foreach(string attrib, boundAttribs)
        if (attrib == name) return;
  Logger::Instance().log("DEBUG", "ShaderProgram", "Binding " + name);
  boundAttribs.push_back(name);
  bindAttrib(name);
}



void ShaderProgram::bindAttrib(string name) {
  Logger::Instance().message << name << " #"<< attribCount;
  Logger::Instance().log("ShaderProgram", "Binding Shader Attribute");
  bindAttrib(attribCount, name);
  attribCount++;
}

void ShaderProgram::defaultAttribs() {
  // TODO(bmonkey): make attribs configurable
  // TODO(bmonkey): Buffer order not variable
  Logger::Instance().message <<
          "Initializing Vertex Attributes for Program #" << program;
    Logger::Instance().log("DEBUG", "Material");
  bindAttrib("in_Vertex");
  // bindAttrib("in_Color");
  bindAttrib("in_Normal");
  bindAttrib("in_Tangent");
  bindAttrib("in_Bitangent");
  bindAttrib("in_Uv");
}

template<typename T>
void ShaderProgram::initUniformsByType(vector<Uniform<T> > & uniforms) {
  foreach(Uniform<T> uniform, uniforms) {
      Logger::Instance().message << "Uniform: " + uniform.name + ": ";
      foreach(T value, uniform.values) {
        Logger::Instance().message << value << ", ";
      }
      Logger::Instance().log("UNIFORMS", "ShaderProgram");


      uniform.init(program);

      Logger::Instance().message << uniform.values.size() << " bound.";
      Logger::Instance().log("UNIFORMS", "ShaderProgram");

      glError;
    }
}

void ShaderProgram::initUniforms() {
  initUniformsByType<float>(uniforms);
  initUniformsByType<int>(uniformsi);
  // TODO: Multiple Light sources
  SceneData::Instance().getShadowLight()->bindShaderInit(this);
}

void ShaderProgram::linkAndUse() {
    /* Link our program, and set it as being actively used */
    glLinkProgram(program);
    printProgramInfoLog();
    use();
}

void ShaderProgram::init() {
  defaultAttribs();
  linkAndUse();
  Logger::Instance().message <<
      "Initializing Uniforms for Program #" << program;
    Logger::Instance().log("DEBUG", "Material");
    initUniforms();
}


void ShaderProgram::use() {
  glError;
  glUseProgram(program);
  glError;
}

void ShaderProgram::reload() {
    glUseProgram(0);
    program = glCreateProgram();

    foreach(Shader* shader, shaders) {
      /*

        delete shader;
        */
      shader->reload();
      // attachShader(shader->fileName,shader->type);
    }

    linkAndUse();
  // ShaderFactory::Instance().addUniforms();
}

void ShaderProgram::setUniform(const QVector4D& vector, string name) {
  glUniform4f(glGetUniformLocation(program, name.c_str()),
          vector.x(), vector.y(), vector.z(), vector.w());
}

void ShaderProgram::setUniform(const QVector3D& vector, string name) {
  glUniform3f(glGetUniformLocation(program, name.c_str()),
          vector.x(), vector.y(), vector.z());
}

void ShaderProgram::setUniform(const QVector2D& vector, string name) {
  glUniform2f(glGetUniformLocation(program, name.c_str()),
          vector.x(), vector.y());
}

GLuint ShaderProgram::getReference() const {
  return program;
}

void ShaderProgram::setUniform(const QMatrix3x3 & matrix, string name) {
    GLfloat mat[9];
    const qreal *data = matrix.constData();
    for (int i = 0; i < 9; ++i)
        mat[i] = data[i];

  glUniformMatrix3fv(
      glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, mat);
}

void ShaderProgram::setUniform(const QMatrix4x4 & matrix, string name) {
    GLfloat mat[16];
    const qreal *data = matrix.constData();
    for (int i = 0; i < 16; ++i)
        mat[i] = data[i];

  glUniformMatrix4fv(
      glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
      // reinterpret_cast<const GLfloat *>(matrix.constData())
      mat);
  /* DOUBLE PRECISION
  glUniformMatrix4dv(
      glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, matrix.data()
  );
  */
}

void ShaderProgram::setUniform(float value, string name) {
  glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void ShaderProgram::translateUniformf(unsigned id, const vector<float> & values){
//	Logger::Instance().log("DEBUG", "translate Uniform name", uniforms[id].name);
  unsigned smaller = (uniforms[id].values.size() < values.size()) ? uniforms[id].values.size() : values.size();
  for (unsigned i = 0; i < smaller; i++)
    uniforms[id].values[i] += values[i];
  uniforms[id].init(program);
}

void ShaderProgram::bindUniformBuffer(string name, GLuint bindIndex, GLuint bufferHandle) {
  glBindBufferBase(GL_UNIFORM_BUFFER, bindIndex, bufferHandle);
  GLuint blockIndex = glGetUniformBlockIndex(program, name.c_str());
  if(blockIndex == GL_INVALID_INDEX)
    Logger::Instance().log("ERROR", "Uniform Buffer not found in Shader", name);
  glUniformBlockBinding(program, blockIndex, bindIndex);
  glError;
}
