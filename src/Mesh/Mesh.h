/*
 * Copyright © 2010 Lubosz Sarnecki
 * Mesh.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "Renderer/OpenGL.h"
#include <string>
#include <QMap>
#include <QList>
#include <vector>
#include <QVector3D>
#include "Mesh/AABB.h"
#include "Mesh/MeshBuffer.h"

using std::string;
using std::vector;

class Mesh {
 public:

  string name;
  QMap<string,vector<GLfloat> > buffers;
  vector<GLuint>  indices;
  vector<Mesh*> subMeshes;
  QMap<string, MeshBuffer* > meshBuffers;
  bool initialized;
	QList<string> usedAttributes;

	Mesh(const QList<string> & attributes);
	void setDrawType(GLint drawType);
	void draw();
	void draw(unsigned amount);
	void init();
	void initBuffer(string name, string linkage, unsigned vertexSize);
	void addSubMesh(Mesh * mesh);
	virtual ~Mesh();
    void vertex(vector<GLfloat> * buffer, const GLfloat &x, const GLfloat &y, const GLfloat &z);
    void vertex(vector<GLfloat> * buffer, const GLfloat &x, const GLfloat &y);
    void vertex(const string & type, const GLfloat &x, const GLfloat &y, const GLfloat &z);
    void vertex(const string & type, GLfloat x, GLfloat y);
    void vertex(const string & type, const QVector3D &vector);

    void makeLinearIndex();
    void append(const string & bufferName, const vector<GLfloat> & vertices);
    void updateBuffer(const string & name);

    void calculatePlaneIndex(unsigned resolution);
    QVector3D getVectorAtPosition(unsigned p, const string &bufferName);
    const unsigned getPositionAtXY(unsigned x, unsigned y, const unsigned resolution) const;
    void calculateNormals(unsigned resolution);
    Mesh * makeNormalMesh();
    QVector3D getTriangleNormal(const QVector3D & p1, const QVector3D & p2, const QVector3D & p3);

    const QVector3D getCenter();

    AABB * getBoundingBox();

 private:
    AABB * boundingBox;
	GLuint vao;
	GLint drawType;

	unsigned indexSize;
	unsigned bufferCount;

	void addBuffer(const vector<GLfloat> &content, unsigned size, string name);
	void addElementBuffer(const vector<GLuint> &content);
};

