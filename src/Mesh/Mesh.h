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

	Mesh();
	void setDrawType(GLint drawType);
	void draw();
	void draw(unsigned amount);
	void init();
	void addSubMesh(Mesh * mesh);
  void addPosition(GLfloat x, GLfloat y, GLfloat z);
  void addColor(GLfloat x, GLfloat y, GLfloat z);
  void addNormal(GLfloat x, GLfloat y, GLfloat z);
  void addTangent(GLfloat x, GLfloat y, GLfloat z);
  void addBitangent(GLfloat x, GLfloat y, GLfloat z);
  void addUv(GLfloat x, GLfloat y);
	virtual ~Mesh();

 private:
	GLuint vao;
	GLint drawType;

	unsigned indexSize;
	unsigned bufferCount;

  void addVertex(GLfloat x, GLfloat y, GLfloat z, vector<GLfloat> * buffer);
	void addBuffer(const vector<GLfloat> &content, unsigned size, string name);
	void addElementBuffer(const vector<GLuint> &content);
};

