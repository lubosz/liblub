/*
 * Geomerty.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include "Geometry.h"
#include <iostream>

using namespace std;

Mesh * Geometry::makeTetrahedron(){
	vector<GLfloat> vertices = {
		1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,
		-1.0,  1.0, -1.0,
		1.0, -1.0, -1.0
	};

	vector<GLfloat> vertexColors = {
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0
	};

	vector<GLfloat> uvCoords = {
		1.0,  0.0,
		0.0,  1.0,
		0.0,  0.0,
		1.0,  1.0
	};

	vector<GLfloat> normals = {
		1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,
		-1.0,  1.0, -1.0,
		1.0, -1.0, -1.0
	};

	vector<GLuint> indicies = { 0, 1, 2, 3, 0, 1 };

	cout << "Making Tetrahedron!!";

	Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    mesh->addBuffer(vertexColors,3,"in_Color");
    mesh->addBuffer(normals,3,"in_Normal");
    mesh->addBuffer(uvCoords,2,"in_Uv");
    mesh->addElementBuffer(indicies);
    mesh->setDrawType(GL_TRIANGLE_STRIP);
	return mesh;

}

Mesh * Geometry::makeCube(){

	vector<GLfloat> vertices = {
			1.0, 1.0, 1.0,
			1.0, -1.0, 1.0,
			-1.0, -1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0,
			1.0, 1.0, -1.0,
			1.0, -1.0, -1.0,
			-1.0, -1.0, -1.0
	};

	vector<GLfloat> vertexColors = {
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0,
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0
	};

	vector<GLfloat> uvCoords = {
		1.0,  0.0,
		0.0,  1.0,
		0.0,  0.0,
		1.0,  1.0,
		1.0,  0.0,
		0.0,  1.0,
		0.0,  0.0,
		1.0,  1.0
	};

	vector<GLuint> indicies = { 0, 1, 2, 3, 4, 5, 6, 7 };

	Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    mesh->addBuffer(vertexColors,3,"in_Color");
    mesh->addBuffer(uvCoords,2,"in_Uv");
    mesh->addElementBuffer(indicies);
	return mesh;

}
