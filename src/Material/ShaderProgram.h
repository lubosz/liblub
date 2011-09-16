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
    void attachShader(string fileName, GLenum type, bool useTemplate);
    void attachShader(string fileName, GLenum type,
            const vector<string> & defines);

    void attachVertFrag(string file, bool useTemplate);
    void attachVertFrag(string file, const vector<string> & defines);
    void attachVertGeom(string file, bool useTemplate);
    void attachVertFragGeom(string file, bool useTemplate);

    void use();
    void bindAttribIfUnbound(string name);
    void bindAttrib(string name);
    void bindAttrib(unsigned position, string name);
    void bindVertexAttributes(const QList<string> & attributes);
    void init(const QList<string> & attributes);
    void linkAndUse();
    GLuint getHandle() const;

    string name;
    vector<Uniform<float> > uniforms;
    vector<Uniform<int> > uniformsi;
    vector<UniformBuffer> uniformBuffers;
    void setUniform(string name, int value);
    void setUniform(string name, float value);
    void setUniform(string name, qreal value);
    void setUniform(string name, const QMatrix3x3 & matrix);
    void setUniform(string name, const QMatrix4x4 & matrix);
    void setUniform(string name, const QVector2D & vector);
    void setUniform(string name, const QVector3D & vector);
    void setUniform(string name, const QVector4D & vector);

    void translateUniformf(unsigned id, const vector<float> & values);

    void initUniforms();
    void bindUniformBuffer(string name, GLuint bindIndex, GLuint bufferHandle);

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

    void addTexture(string file, string name);
    void addTextureCube(string file, string name);

private:
    /* This is a handle to the shader program */
    GLuint handle;
    template<typename T> void
            initUniformsByType(vector<Uniform<T> > & uniforms);
    vector<Shader*> shaders;
    vector<string> boundAttribs;
    unsigned attribCount;
    void printProgramInfoLog();
    void detachShader(Shader *shader);
};
