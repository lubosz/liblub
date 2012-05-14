/*
 * Copyright © 2010 Lubosz Sarnecki
 * ShaderSystem.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include <string>
#include <QList>

#include "common/Qt3D.h"
#include "Material/Shader.h"
#include "Material/Uniform.h"
#include "Material/UniformBuffer.h"
#include "Material/Texture.h"

using std::string;

class ShaderProgram {
public:
    ShaderProgram();
    virtual ~ShaderProgram();
    void attachShader(const string &fileName, GLenum type, bool useTemplate);
    void attachShader(const string &fileName, GLenum type,
            const vector<string> & defines);

    void attachVertFrag(const string &file, bool useTemplate);
    void attachVertFrag(const string &file, const vector<string> & defines);
    void attachVertGeom(const string &file, bool useTemplate);
    void attachVertFragGeom(const string &file, bool useTemplate);

    void use();
    void bindAttribIfUnbound(const string &name);
    void bindAttrib(const string &name);
    void bindAttrib(unsigned position, const string &name);
    void bindVertexAttributes(const QList<string> & attributes);
    void init(const QList<string> & attributes);
    void linkAndUse();
    GLuint getHandle() const;

    string name;
    vector<Uniform<float> > uniforms;
    vector<Uniform<int> > uniformsi;
    vector<UniformBuffer> uniformBuffers;
    void setUniform(const string &name, int value);
    void setUniform(const string &name, float value);
    void setUniform(const string &name, qreal value);
    void setUniform(const string &name, const QMatrix3x3 & matrix);
    void setUniform(const string &name, const QMatrix4x4 & matrix);
    void setUniform(const string &name, const QVector2D & vector);
    void setUniform(const string &name, const QVector3D & vector);
    void setUniform(const string &name, const QVector4D & vector);

    void translateUniformf(unsigned id, const vector<float> & values);

    void initUniforms();
    #ifdef USE_OPENGL3
    void bindUniformBuffer(const string &name, GLuint bindIndex, GLuint bufferHandle);
    #endif
    void samplerUniforms();
    void addTexture(Texture * texture);
    void addTextures(const vector<Texture *> &addTextures);

    vector<Texture*> textures;

    void init();
    void initRenderTargets(const vector<Texture*> &targets);
    void activateAndBindTextures();
    void bindTextures();
    void activateTextures();
protected:

    void addTexture(const string &file, const string &name);
    void addTextureCube(const string &file, const string &name);

private:
    /* This is a handle to the shader program */
    GLuint handle;
    template<typename T> void
            initUniformsByType(const vector<Uniform<T> > & uniforms);
    vector<Shader*> shaders;
    vector<string> boundAttribs;
    unsigned attribCount;
    void printProgramInfoLog();
    void detachShader(Shader *shader);
};
