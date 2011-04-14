/*
 * Copyright © 2010 Lubosz Sarnecki
 * Mesh.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "common/OpenGL.h"
#include <string>
#include <vector>
#include <QVector3D>
#include "Mesh/AABB.h"

using std::string;
using std::vector;

// TODO(bmonkey): This is a very bad solution (and also hardcoded)
const unsigned maxBuffers = 6;

class Mesh {
 public:

  GLuint index;

  vector<GLfloat> positions;
  vector<GLfloat> normals;
  vector<GLfloat> tangents;
  vector<GLfloat> binormals;
  vector<GLfloat> uvs;
  vector<GLuint>  indices;

  vector<Mesh*> subMeshes;

	Mesh();
	void addBuffer(const vector<GLfloat> &content, unsigned size, string name);
	void addElementBuffer(const vector<GLuint> &content);
	void setDrawType(GLint drawType);
	void draw();
	void draw(unsigned amount);
	void init();
	void addSubMesh(Mesh * mesh) {
	  subMeshes.push_back(mesh);
	}
	void initPositions() {
	  addBuffer(positions, 3, "in_Vertex");
	}
	void initNormals(){
    addBuffer(normals, 3, "in_Normal");
  }
  void initUv(){
    addBuffer(uvs, 2, "in_Uv");
  }
	void initIndex(){
    addElementBuffer(indices);
  }

  void addVertex(GLfloat x, GLfloat y, GLfloat z, vector<GLfloat> * buffer) {
    buffer->push_back(x);
    buffer->push_back(y);
    buffer->push_back(z);
  }

  void addPosition(GLfloat x, GLfloat y, GLfloat z){
    addVertex(x, y, z, &positions);
    indices.push_back(index);
    index++;
  }

  void addNormal(GLfloat x, GLfloat y, GLfloat z){
//    printf("Normal %f, %f, %f\n",x,y,z);
    addVertex(x, y, z, &normals);
  }

  void addUv(GLfloat x, GLfloat y){
    uvs.push_back(x);
    uvs.push_back(y);
  }

	AABB * boundingBox;

	virtual ~Mesh();

 private:
	/* Create handles for a Vertex Array Object
	 * and 6 Vertex Buffer Objects */

	GLuint vao, vbo[maxBuffers];
	GLint drawType;

	unsigned indexSize;
	unsigned bufferCount;

};

