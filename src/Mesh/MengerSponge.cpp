/*
 * MengerSponge.cpp
 *
 *  Created on: Jul 11, 2010
 *      Author: bmonkey
 */

#include "MengerSponge.h"
#include <list>

MengerSponge::MengerSponge(unsigned recursion) {
	vector<GLfloat> vertices = {
			1.0, -1.0, -1.0,
			1.0, -1.0, 1.0,
			-1.0, -1.0, 1.0,
			-1.0, -1.0, -1.0,
			1.0, 1.0, -1.0,
			1.0, 1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0
	};

	vector<GLfloat> color = {
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 1.0, 0.0,
			1.0, 0.0, 1.0,
			1.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0
	};

	vector<GLfloat> normals = {};

	vector<GLfloat> uvCoords = {
			1.0, 0.0,
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0

	};

	vector<GLuint> indicies;
	vector<GLuint> back = {
			4, 0, 3,
			4, 3, 7,
				};
	indicies.insert(indicies.end(), back.begin(), back.end());

	vector<GLuint> front = {
			1,5,2,
			5,6,2,
				};
	indicies.insert(indicies.end(), front.begin(), front.end());

	vector<GLuint> left = {
			2, 6, 7,
			2, 7, 3,
				};
	indicies.insert(indicies.end(), left.begin(), left.end());

	vector<GLuint> right = {
			0,4,1,
			4,5,1,
				};
	indicies.insert(indicies.end(), right.begin(), right.end());

	vector<GLuint> top = {
			4,7,5,
			7,6,5,
				};
	indicies.insert(indicies.end(), top.begin(), top.end());

	vector<GLuint> bottom = {
			0,1,2,
			0,2,3
				};
	indicies.insert(indicies.end(), bottom.begin(), bottom.end());


	mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    mesh->addBuffer(color,3,"in_Color");
    mesh->addBuffer(vertices,3,"in_Normal");
    mesh->addBuffer(uvCoords,2,"in_Uv");
    mesh->addElementBuffer(indicies);
    mesh->setDrawType(GL_TRIANGLES);

}

MengerSponge::~MengerSponge() {
	// TODO Auto-generated destructor stub
}

Mesh * MengerSponge::getMesh(){
	return mesh;
}
