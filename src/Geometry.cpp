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
			/*
			0.4, -0.3, -0.8,
			0.4, -0.3, 0.8,
			-0.8, -0.5, 0.0,
			0.0, 0.9, 0.0
			*/
	};

	vector<GLfloat> binormals = {
		    1.0,  1.0,  1.0,
		    -1.0, -1.0,  1.0,
		    -1.0,  1.0, -1.0,
		    1.0, -1.0, -1.0
			/*
			0.4, -0.3, -0.8,
			0.4, -0.3, 0.8,
			-0.8, -0.5, 0.0,
			0.0, 0.9, 0.0
			*/
	};

	vector<GLfloat> tangents = {
		    1.0,  1.0,  1.0,
		    -1.0, -1.0,  1.0,
		    -1.0,  1.0, -1.0,
		    1.0, -1.0, -1.0
			/*
			0.4, -0.3, -0.8,
			0.4, -0.3, 0.8,
			-0.8, -0.5, 0.0,
			0.0, 0.9, 0.0
			*/
	};

	vector<GLuint> indicies = { 0, 1, 2, 3, 0, 1 };

	cout << "Making Tetrahedron!!";
	//return new Mesh(vertices,vertexColors,normals,binormals,tangents,uvCoords,indicies);
	Mesh * mesh = new Mesh();
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

	//return new Mesh(vertices,vertexColors,vertices,vertices,vertices,uvCoords,indicies);
	Mesh * mesh = new Mesh();
	return mesh;

}
