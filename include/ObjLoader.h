/*
 * ObjLoader.h
 *
 *  Created on: Jun 2, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "Mesh.h"
const string meshDir = "media/meshes/";

class ObjLoader {
public:

	ObjLoader();
	~ObjLoader();

	Mesh* readFile(string file);

	Mesh* mesh;
	string objSource;

	void addTriangle(unsigned faceIndex,vector<GLfloat> * hardCopy,vector<GLfloat> * coords,vector<unsigned> * index, unsigned length);
	void addPoint(vector<GLfloat> * hardCopy, vector<GLfloat> * coords, unsigned firstCoord, unsigned length);
	void printVector(vector<GLfloat> * printMe, unsigned length);
	void reOrderHardCopy(vector<GLfloat> &vertices,vector<GLfloat> &normals,vector<GLfloat> &uvCoords,vector<GLuint> &vertIndex,vector<GLuint> &normalIndex,vector<GLuint> &uvIndex);
	void decrementIndex(vector<GLuint> &indices);
	void createMesh(vector<GLfloat> &vertices,vector<GLfloat> &normals,vector<GLfloat> &uvCoords,vector<GLuint> &indices);




};
