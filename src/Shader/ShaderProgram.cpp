/*
 * Copyright © 2010 Lubosz Sarnecki
 * ShaderSystem.cpp
 *
 *  Created on: Mar 22, 2010
 */
#include <assert.h>
#include <QVector2D>
#include "Shader/ShaderProgram.h"
#include "Scene/SceneGraph.h"
#include "System/Logger.h"
#include "Scene/Scene.h"
#include "Renderer/OpenGL.h"
#include "Texture/Textures.h"

using std::stringstream;

ShaderProgram::ShaderProgram(string name) : NamedObject(name) {
  attribCount = 0;
  handle = glCreateProgram();
  LogDebug << "Creating Program #" << handle;
  textures = vector<Texture*>();
}

ShaderProgram::~ShaderProgram() {
    foreach (Shader* shader, shaders) {
        detachShader(shader);
    }
}

void ShaderProgram::printProgramInfoLog() {
  int infologLen = 0;
  glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infologLen);
  if (infologLen > 1) {
    GLchar * infoLog = new GLchar[infologLen];
    if (infoLog == NULL) {
      LogError << "Could not allocate InfoLog buffer";
    }
    int charsWritten = 0;
    glGetProgramInfoLog(handle, infologLen, &charsWritten, infoLog);
    string shaderlog = infoLog;
    free(infoLog);
    foreach(Shader * shader, shaders) {
        LogError << "======" << shader->fileName;
        QString shaderSource = QString::fromStdString(shader->shaderSource);
        QStringList shaderSourceLines = shaderSource.split("\n");
        unsigned lineNumber = 0;
        foreach(QString line, shaderSourceLines){
//            if(!line.contains("#", Qt::CaseInsensitive)) {
                LogError << lineNumber << line.toStdString();
                lineNumber++;
//            }
        }
    }
    LogError << "Program Log"<< shaderlog;
  } else {
    LogDebug << "Program compiled";
  }
}

void ShaderProgram::attachShader(const string &fileName, GLenum type) {
    /* Attach our shaders to our program */
  Shader * shader = new Shader(fileName, type);
  shaders.push_back(shader);
  glAttachShader(handle, shader->getHandle());
}

void ShaderProgram::detachShader(Shader *shader) {
  glDetachShader(handle, shader->getHandle());
//  shaders.remove(shader);
  delete shader;
}

void ShaderProgram::attachVertFrag(const string &file) {
  attachShader(file + ".vert", GL_VERTEX_SHADER);
  attachShader(file + ".frag", GL_FRAGMENT_SHADER);
}

void ShaderProgram::attachVertGeom(const string &file) {
  attachShader(file + ".vert", GL_VERTEX_SHADER);
  attachShader(file + ".geom", GL_GEOMETRY_SHADER);
}

void ShaderProgram::attachVertFragGeom(const string &file) {
  attachShader(file + ".vert", GL_VERTEX_SHADER);
  attachShader(file + ".geom", GL_GEOMETRY_SHADER);
  attachShader(file + ".frag", GL_FRAGMENT_SHADER);
}

void ShaderProgram::bindAttrib(unsigned position, const string &name) {
  glBindAttribLocation(handle, position, name.c_str());
}

void ShaderProgram::bindAttribIfUnbound(const string &name) {
  foreach(string attrib, boundAttribs)
        if (attrib == name) return;
  LogDebug << "Binding " + name;
  boundAttribs.push_back(name);
  bindAttrib(name);
}



void ShaderProgram::bindAttrib(const string &name) {
  LogDebug << "Binding Shader Attribute" << name << " #"<< attribCount;
  bindAttrib(attribCount, name);
  attribCount++;
}

void ShaderProgram::bindVertexAttributes(const QList<string> & attributes) {
//Order has to match the shader

  LogDebug << "Initializing Vertex Attributes for Program #" << handle;

  bindAttrib("in_Vertex");

  if(attributes.contains("color"))
    bindAttrib("in_Color");

  if(attributes.contains("normal"))
    bindAttrib("in_Normal");

  if(attributes.contains("tangent"))
    bindAttrib("in_Tangent");

  if(attributes.contains("bitangent"))
    bindAttrib("in_Bitangent");

 if(attributes.contains("uv"))
    bindAttrib("in_Uv");
}

template<typename T>
void ShaderProgram::initUniformsByType(const vector<Uniform<T> > & uniforms) {
  foreach(Uniform<T> uniform, uniforms) {
    stringstream log;
      log << "Uniform: " + uniform.name + ": ";
      foreach(T value, uniform.values) {
        log << value << ", ";
      }
      uniform.init(handle);
      LogDebug << log.str() << uniform.values.size() << " bound.";
      glError;
    }
}

void ShaderProgram::initUniforms() {
  initUniformsByType<float> (uniforms);
  initUniformsByType<int> (uniformsi);
  // TODO: Multiple Light sources
  Scene::Instance().getDefaultLight()->bindShaderInit(this);
}

void ShaderProgram::linkAndUse() {
  /* Link our program, and set it as being actively used */
  glLinkProgram(handle);
  printProgramInfoLog();
  use();
}

void ShaderProgram::init(const QList<string> & attributes) {
  bindVertexAttributes(attributes);
  linkAndUse();
  LogDebug << "Initializing Uniforms for Program #" << handle;
  initUniforms();
}

void ShaderProgram::use() {
  glError;
  glUseProgram(handle);
//  foreach(UniformBuffer* buffer, uniformBuffers) {
//      bindUniformBuffer(buffer->getName(),0,buffer->getHandle());
//  }
  glError;
}

GLint ShaderProgram::getUniformLocation(const string &name) const {
    return glGetUniformLocation(handle, name.c_str());
}

void ShaderProgram::setUniform(const string &name, const QVector4D& vector) {
  glUniform4f(getUniformLocation(name),
          vector.x(), vector.y(), vector.z(), vector.w());
}

void ShaderProgram::setUniform(const string &name, const QVector3D& vector) {
  glUniform3f(getUniformLocation(name),
          vector.x(), vector.y(), vector.z());
}

void ShaderProgram::setUniform(const string &name, const QVector2D& vector) {
  glUniform2f(getUniformLocation(name),
          vector.x(), vector.y());
}

GLuint ShaderProgram::getHandle() const {
  return handle;
}

void ShaderProgram::setUniform(const string &name, const QMatrix3x3 & matrix) {
    GLfloat mat[9];
    const qreal *data = matrix.constData();
    for (int i = 0; i < 9; ++i)
        mat[i] = data[i];

  glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, mat);
}

void ShaderProgram::setUniform(const string &name, const QMatrix4x4 & matrix) {
    GLfloat mat[16];
    const qreal *data = matrix.constData();
    for (int i = 0; i < 16; ++i)
        mat[i] = data[i];

  glUniformMatrix4fv(
      getUniformLocation(name), 1, GL_FALSE,
      // reinterpret_cast<const GLfloat *>(matrix.constData())
      mat);
  /* DOUBLE PRECISION
  glUniformMatrix4dv(
      glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, matrix.data()
  );
  */
}

void ShaderProgram::setUniform(GLint uniform, const QMatrix3x3 & matrix) {
    GLfloat mat[9];
    const qreal *data = matrix.constData();
    for (int i = 0; i < 9; ++i)
        mat[i] = data[i];

    glUniformMatrix3fv(uniform, 1, GL_FALSE, mat);
}

void ShaderProgram::setUniform(GLint uniform, const QMatrix4x4 & matrix) {
    GLfloat mat[16];
    const qreal *data = matrix.constData();
    for (int i = 0; i < 16; ++i)
        mat[i] = data[i];

    glUniformMatrix4fv(uniform, 1, GL_FALSE, mat);
}

void ShaderProgram::setUniform(const string &name, float value) {
  glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const string &name, qreal value) {
    setUniform(name, static_cast<float>(value));
}

void ShaderProgram::setUniform(const string &name, int value) {
  glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::translateUniformf(unsigned id, const vector<float> & values){
//	Logger::Instance().log("DEBUG", "translate Uniform name", uniforms[id].name);
	for (unsigned i = 0; i < values.size(); i++)
		uniforms[id].values[i] += values[i];
	uniforms[id].init(handle);
}
#ifdef USE_OPENGL3
void ShaderProgram::bindUniformBuffer(const string &name, GLuint bindIndex, GLuint bufferHandle) {
  glBindBufferBase(GL_UNIFORM_BUFFER, bindIndex, bufferHandle);
  GLuint blockIndex = glGetUniformBlockIndex(handle, name.c_str());
  if(blockIndex == GL_INVALID_INDEX)
    LogFatal << "Uniform Buffer not found in Shader" << name;
  glUniformBlockBinding(handle, blockIndex, bindIndex);
  glError;
}
#endif
void ShaderProgram::addTexture(Texture * texture) {
    textures.push_back(texture);
}

void ShaderProgram::addTextures(const vector<Texture *> &addTextures) {
  foreach(Texture * texture, addTextures)
    textures.push_back(texture);
}

void ShaderProgram::addTexture(const string &file, const string &name) {
    textures.push_back(new TextureFile(file, name));
}
void ShaderProgram::addTextureCube(const string &file, const string &name) {
    textures.push_back(new CubeTextureFile(file, name));
}

void ShaderProgram::activateAndBindTextures() {
    foreach(Texture* texture, textures) {
            texture->activate();
            texture->bind();
            texture->uniform(getHandle());
        }
    glError;
}

void ShaderProgram::activateTextures() {
    foreach(Texture* texture, textures) {
            texture->activate();
        }
    glError;
}

void ShaderProgram::bindTextures() {
    foreach(Texture* texture, textures) {
            texture->bind();
        }
    glError;
}

void ShaderProgram::initRenderTargets(const vector<Texture*> &targets){
  for(unsigned i = 0; i < targets.size(); i++){
    if(!targets[i]->isDepth)
      glBindFragDataLocation(handle, i, targets[i]->name.c_str());
  }
}

void ShaderProgram::samplerUniforms() {
    foreach(Texture* texture, textures) {
            texture->uniform(handle);
        }
}
