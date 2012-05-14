/*
 * Copyright © 2010 Lubosz Sarnecki
 * Mesh.cpp
 *
 *  Created on: Mar 22, 2010
 */
#include <string>
#include <vector>
#include <assert.h>
#include "Mesh/Mesh.h"
#include "Renderer/OpenGL.h"
#include "System/Logger.h"

Mesh::Mesh(const QList<string> & attributes) {
  usedAttributes = attributes;
  drawType = GL_POINTS;
  bufferCount = 0;
  indexSize = 0;
  indices = vector<GLuint>();
  initialized = false;
  boundingBox = nullptr;

  meshBuffers = QMap<string, MeshBuffer* > ();

  buffers = QMap<string, vector<GLfloat> > ();
  buffers["position"] = vector<GLfloat>();

  foreach(string attrib, attributes)
      buffers[attrib] = vector<GLfloat>();
}

void Mesh::init() {
  LogDebug << "Buffersize" << buffers.count() << "Indexsize" << indices.size();
  assert(!buffers.empty());
  assert(!buffers["position"].empty());
//  assert(!indices.empty());
  assert(OpenGL::Instance().getContextCreated());
  /* Allocate and assign a Vertex Array Object to our handle */
  glGenVertexArrays(1, &vao);
  LogDebug << "Generating Vertex Array Object #" << vao;

  /* Bind our Vertex Array Object as the current used object */
  glBindVertexArray(vao);

  addBuffer(buffers["position"], 3, "in_Vertex");
  initBuffer("color", "in_Color", 3);
  initBuffer("normal", "in_Normal", 3);
  initBuffer("tangent", "in_Tangent", 3);
  initBuffer("bitangent", "in_Bitangent", 3);
  initBuffer("uv", "in_Uv", 2);

  if(indices.empty())
      makeLinearIndex();

  addElementBuffer(indices);
  initialized = true;
}

void Mesh::initBuffer(string name, string linkage, unsigned vertexSize) {
  if(usedAttributes.contains(name)) {
    if(!buffers[name].empty()) {
      addBuffer(buffers[name], vertexSize, linkage);
    } else {
      if (vertexSize == 3) {
        LogWarning << "No" << name << "Buffer found. Will use Position Buffer instead.";
        addBuffer(buffers["position"], vertexSize, linkage);
      } else {
        LogError << "No" << name << "Buffer found.";
      }
    }
  }
}

Mesh::~Mesh() {
    if(initialized) {
        glBindVertexArray(vao);
        for (unsigned i = 0; i < bufferCount; i++) {
          glDisableVertexAttribArray(i);
        }
//    glDeleteBuffers(bufferCount, vbo);
        glDeleteVertexArrays(1, &vao);
    }
    foreach(Mesh * subMesh, subMeshes)
        delete subMesh;
    if(boundingBox != nullptr)
        delete boundingBox;
}

void Mesh::addBuffer(const vector<GLfloat> &content, unsigned size, string name) {
    glError;

    meshBuffers[name] = new MeshBuffer(name, bufferCount, size);

    meshBuffers[name]->init();
    meshBuffers[name]->write_dynamic(content);

  bufferCount++;
    glError;
}

void Mesh::updateBuffer(const string & name) {
    if(!meshBuffers.contains(name)) {
        LogError << name << "buffer does not exist";
        foreach(string buffername, meshBuffers.keys())
            LogError << buffername;
    }

    meshBuffers[name]->write_dynamic(buffers[name]);
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

#ifdef USE_OPENGL3
    glBindVertexArray(vao);
    glDrawElements(drawType, indexSize, GL_UNSIGNED_INT, 0);
#endif
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

void Mesh::makeLinearIndex() {
    for(unsigned i = 0; i < buffers["position"].size(); i+= 3) {
        indices.push_back(i/3);
    }
}

void Mesh::vertex(const string & type, const GLfloat &x, const GLfloat &y, const GLfloat &z) {

//    LogInfo << type;
//  if(!buffers.contains(type))
//    buffers[type] = vector<GLfloat>();

  vector<GLfloat> * buffer = &buffers[type];

  buffer->push_back(x);
  buffer->push_back(y);
  buffer->push_back(z);
}

void Mesh::vertex(const string & type, const QVector3D & vector) {
  vertex(type, vector.x(), vector.y(), vector.z());
}

//currently used only for uv buffer
void Mesh::vertex(const string & type, GLfloat x, GLfloat y) {
  buffers[type].push_back(x);
  buffers[type].push_back(y);
}

void Mesh::append(const string &bufferName, const vector<GLfloat> &vertices){
    if (buffers.contains(bufferName)) {
    buffers[bufferName].insert(
            buffers[bufferName].end(), vertices.begin(), vertices.end());
    } else {
        LogError << "Buffer not found" << bufferName;
    }
}

void Mesh::calculatePlaneIndex(unsigned resolution) {
    for(unsigned iY=0; iY < resolution - 1; iY++) {

           if ( (iY&1)==0 ) { // even iYs
               for ( unsigned iX=0; iX<resolution; iX ++ ) {
                   indices.push_back(iX + iY * resolution);
                   indices.push_back(iX + (iY+1) * resolution);
               }
           } else { // odd iYs
               for ( unsigned iX = resolution - 1; iX > 0; iX-- ) {
                   indices.push_back(iX + (iY+1) * resolution);
                   indices.push_back(iX - 1 + iY * resolution);
               }
           }
    }
}

QVector3D Mesh::getTriangleNormal(const QVector3D & p1, const QVector3D & p2, const QVector3D & p3) {
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p1;

    QVector3D normal = QVector3D::crossProduct(v1, v2);
    normal.normalize();

    return -normal;
}

QVector3D Mesh::getVectorAtPosition(unsigned p, const string & bufferName) {
    vector<GLfloat> * buffer = &buffers[bufferName];
    p = p * 3;
    if (p + 2 > buffer->size())
        return QVector3D(0, 1, 0);
    return QVector3D(buffer->at(p), buffer->at(p+1), buffer->at(p+2));
}

const unsigned Mesh::getPositionAtXY(unsigned x, unsigned y, unsigned resolution) const {
//      if (x < 0) x = 0;
//      if (y < 0) y = 0;
    if (x > resolution) x = resolution;
    if (y > resolution) y = resolution;

    return x + y * resolution;
}

void Mesh::calculateNormals(unsigned resolution) {
    for(unsigned y=0; y < resolution; y++) {
       for(unsigned x=0; x < resolution; x++) {
           QVector3D p1 = getVectorAtPosition(getPositionAtXY(x, y, resolution), "position");
           QVector3D p2 = getVectorAtPosition(getPositionAtXY(x-1, y, resolution), "position");
           QVector3D p3 = getVectorAtPosition(getPositionAtXY(x, y - 1, resolution), "position");

           QVector3D normal = getTriangleNormal(p1, p2, p3);

           QVector3D p4 = getVectorAtPosition(getPositionAtXY(x+1, y, resolution), "position");
           QVector3D p5 = getVectorAtPosition(getPositionAtXY(x, y+1, resolution), "position");

           QVector3D normal2 = getTriangleNormal(p1, p4, p5);

           normal = (normal + normal2 )/ 2;

//             LogInfo << normal.x() << normal.y() << normal.z();
           vertex("normal", normal);
        }
    }
}


Mesh * Mesh::makeNormalMesh() {

    QList<string> NormalAttributes = QList<string>() << "color";
    Mesh * normalMesh = new Mesh(NormalAttributes);

    for(unsigned i=0; i < buffers["position"].size()/3; i++) {
                normalMesh->vertex("position", getVectorAtPosition(i, "position"));
                normalMesh->vertex("position", getVectorAtPosition(i, "position") + getVectorAtPosition(i, "normal"));
                normalMesh->vertex("color", QVector3D(0,1,1));
                normalMesh->vertex("color", QVector3D(0,1,1));
    }

    normalMesh->init();
    normalMesh->setDrawType(GL_LINES);

    return normalMesh;
}
