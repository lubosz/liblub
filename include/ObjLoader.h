/*
 * ObjLoader.h
 *
 *  Created on: Jun 2, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Vector2D.h"
#include "Vector3D.h"

#include "common.h"
#include "Mesh.h"
const string meshDir = "media/meshes/";

class ObjLoader {
public:

	ObjLoader();
	~ObjLoader();

	Mesh* readFile(string file);

	string objSource;

	vector<GLfloat> hardVertices, hardUvCoords, hardNormals;
	vector<GLuint> hardIndex;

	void addTriangle(unsigned faceIndex,vector<GLfloat> * hardCopy,vector<GLfloat> * coords,vector<unsigned> * index, unsigned length);
	void addPoint(vector<GLfloat> * hardCopy, vector<GLfloat> * coords, unsigned firstCoord, unsigned length);
	void printVector(vector<GLfloat> * printMe, unsigned length);
	void reOrderHardCopy(vector<GLfloat> &vertices,vector<GLfloat> &normals,vector<GLfloat> &uvCoords,vector<GLuint> &vertIndex,vector<GLuint> &normalIndex,vector<GLuint> &uvIndex);
	void decrementIndex(vector<GLuint> &indices);
	void hardCopyIndex(vector<Vector3D> &vertices,vector<Vector3D> &normals,vector<Vector2D> &uvCoords,vector<GLuint> &vertIndex,vector<GLuint> &normalIndex,vector<GLuint> &uvIndex);
	Mesh * createMesh();




};
