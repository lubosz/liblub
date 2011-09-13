/*
 * Copyright © 2010 Lubosz Sarnecki
 * Mesh.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "common/OpenGL.h"
#include <string>
#include <QMap>
#include <QList>
#include <vector>
#include <QVector3D>
#include "Mesh/AABB.h"

using std::string;
using std::vector;

class Mesh {
 public:

  string name;
  GLuint index;
  QMap<string,vector<GLfloat> > buffers;
  vector<GLuint>  indices;
  vector<Mesh*> subMeshes;
	AABB * boundingBox;
	QList<string> usedAttributes;

	Mesh(const QList<string> & attributes);
	void setDrawType(GLint drawType);
	void draw();
	void draw(unsigned amount);
	void init();
	void initBuffer(string name, string linkage, unsigned vertexSize);
	void addSubMesh(Mesh * mesh);
	virtual ~Mesh();
	void vertex(string type, GLfloat x, GLfloat y, GLfloat z);
	void vertex(string type, GLfloat x, GLfloat y);
	void append(const string bufferName, vector<GLfloat> & vertices);

 private:
	GLuint vao;
	GLint drawType;

	unsigned indexSize;
	unsigned bufferCount;

	void addBuffer(const vector<GLfloat> &content, unsigned size, string name);
	void addElementBuffer(const vector<GLuint> &content);
};

