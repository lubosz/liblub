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

	list<GLuint> blubb;
	list<GLuint> blubb2 = {
			4, 0, 3,
			4, 3, 7,
				};
	blubb.merge(blubb2);
	//blubb.insert(4, 0, 3,4, 3, 7);

	vector<GLuint> indicies = {

									//Back
									4, 0, 3,
									4, 3, 7,
//Front
									1,5,2,
									5,6,2,
//Left

									2, 6, 7,
									2, 7, 3,
//Right
									0,4,1,
									4,5,1,
//Top
									4,7,5,
									7,6,5,
//Bottom
									0,1,2,
									0,2,3


	};

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
