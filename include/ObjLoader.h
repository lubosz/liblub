/*
 * ObjLoader.h
 *
 *  Created on: Jun 2, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Vector2D.h"
#include "Vector3D.h"
#include "Face.h"
#include "common.h"
#include "Mesh.h"


class ObjLoader {
public:
	ObjLoader(string file);
	~ObjLoader();
	Mesh * getMesh() const;
private:
	Mesh * mesh;

	string filePath;

	vector<GLfloat> hardVertices, hardUvCoords, hardNormals;
	vector<GLuint> hardIndex;

	vector<Vector3D> vertices, normals;
	vector<Vector2D> uvCoords;
	vector<Face> faces;

	vector<unsigned> vertIndex, normalIndex, uvIndex;

	unsigned hardIndexSize;

	void readFile();
	void addTriangle(Face face);
	void addPoint(vector<GLfloat> * hardCopy, vector<GLfloat> * coords, unsigned firstCoord, unsigned length);
	void printVector(vector<GLfloat> * printMe, unsigned length);
	//void reOrderHardCopy(vector<GLfloat> &vertices,vector<GLfloat> &normals,vector<GLfloat> &uvCoords,vector<GLuint> &vertIndex,vector<GLuint> &normalIndex,vector<GLuint> &uvIndex);
	void decrementIndex(vector<GLuint> &indices);
	void hardCopyIndex();
	Mesh * createMesh();
};
