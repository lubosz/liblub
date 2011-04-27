/*
 * Copyright © 2010 Lubosz Sarnecki
 * Mesh.cpp
 *
 *  Created on: Mar 22, 2010
 */
#include <string>
#include <vector>
#include <assert.h>
#include <assert.h>
#include "Renderer/RenderEngine.h"
#include "System/Logger.h"

Mesh::Mesh() {
  index = 0;
  drawType = GL_POINTS;
  bufferCount = 0;
  indexSize = 0;
  buffers = QMap<string, vector<GLfloat> > ();
  indices = vector<GLuint>();
}

void Mesh::init() {
  LogDebug << "Buffersize" << buffers.count() << "Indexsize" << indices.size();
  assert(!buffers.empty());
  assert(!buffers["position"].empty());
  assert(!indices.empty());
  /* Allocate and assign a Vertex Array Object to our handle */
  glGenVertexArrays(1, &vao);
  LogDebug << "Generating Vertex Array Object #" << vao;

  /* Bind our Vertex Array Object as the current used object */
  glBindVertexArray(vao);

  addBuffer(buffers["position"], 3, "in_Vertex");
  if(!buffers["normal"].empty())
    addBuffer(buffers["normal"], 3, "in_Normal");
  if(!buffers["tangent"].empty())
    addBuffer(buffers["tangent"], 3, "in_Tangent");
  if(!buffers["bitangent"].empty())
    addBuffer(buffers["bitangent"], 3, "in_Bitangent");
  if(!buffers["uv"].empty())
    addBuffer(buffers["uv"], 2, "in_Uv");
  addElementBuffer(indices);
}

Mesh::~Mesh() {
  glBindVertexArray(vao);
  for (unsigned i = 0; i < bufferCount; i++) {
    glDisableVertexAttribArray(i);
  }
//    glDeleteBuffers(bufferCount, vbo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::addBuffer(const vector<GLfloat> &content, unsigned size, string name) {
    glError;
    GLuint vbo;
    glGenBuffers(1, &vbo);
    /* Bind the first VBO as being the active buffer
     * and storing vertex attributes (coordinates) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

  /* Copy the vertex data from mesh to our buffer */
  /* 12 * sizeof(GLfloat) is the size of the
   * mesh array, since it contains 12 GLfloat values */
  glBufferData(GL_ARRAY_BUFFER, content.size() * sizeof(GLfloat),
            content.data(), GL_STATIC_DRAW);

  /* Specify that our coordinate data is going
   * into attribute index bufferCount, and contains three floats per vertex */
  glVertexAttribPointer((GLuint) bufferCount, size, GL_FLOAT, GL_FALSE, 0, 0);

  /* Enable attribute index 0 as being used */
  glEnableVertexAttribArray(bufferCount);
    LogDebug << "Adding Vertex Buffer #" << bufferCount
            << " " << name << " Size:" << content.size();
  bufferCount++;
    glError;
}

void Mesh::addElementBuffer(const vector<GLuint> & content) {
  indexSize = content.size();
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  glError;
  /* Copy the index data from tetraindicies to our buffer
   * 6 * sizeof(GLubyte) is the size of the index array, since it contains 6 GLbyte values */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, content.size() * sizeof(GLuint),
            content.data(), GL_STATIC_DRAW);
    LogDebug << "Adding Vertex Element Buffer #"
            << bufferCount << " Size:" << content.size();
    bufferCount++;
    glError;
}

void Mesh::setDrawType(GLint drawType) {

  if (subMeshes.size() > 0) {
    foreach(Mesh * mesh, subMeshes)
        mesh->drawType = drawType;
  } else {
    this->drawType = drawType;
  }
}

void Mesh::draw() {
  if (subMeshes.size() > 0) {
    foreach(Mesh * mesh, subMeshes)
        mesh->draw();
  } else {
    glBindVertexArray(vao);
    glDrawElements(drawType, indexSize, GL_UNSIGNED_INT, 0);
    glError;
  }
}

void Mesh::draw(unsigned amount) {
  glBindVertexArray(vao);
  glDrawElementsInstanced(drawType, indexSize, GL_UNSIGNED_INT, 0, amount);
  glError;
}

void Mesh::addSubMesh(Mesh * mesh) {
  subMeshes.push_back(mesh);
}

void Mesh::addVertex(GLfloat x, GLfloat y, GLfloat z, vector<GLfloat> * buffer) {
  buffer->push_back(x);
  buffer->push_back(y);
  buffer->push_back(z);
}

void Mesh::addPosition(GLfloat x, GLfloat y, GLfloat z) {
  addVertex(x, y, z, &buffers["position"]);
  indices.push_back(index);
  index++;
}

void Mesh::addNormal(GLfloat x, GLfloat y, GLfloat z) {
  addVertex(x, y, z, &buffers["normal"]);
}

void Mesh::addTangent(GLfloat x, GLfloat y, GLfloat z) {
  addVertex(x, y, z, &buffers["tangent"]);
}

void Mesh::addBitangent(GLfloat x, GLfloat y, GLfloat z) {
  addVertex(x, y, z, &buffers["bitangent"]);
}

void Mesh::addColor(GLfloat x, GLfloat y, GLfloat z) {
  addVertex(x, y, z, &buffers["color"]);
}

void Mesh::addUv(GLfloat x, GLfloat y) {
  buffers["uv"].push_back(x);
  buffers["uv"].push_back(y);
}
