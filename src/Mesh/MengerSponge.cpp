/*
 * MengerSponge.cpp
 *
 *  Created on: Jul 11, 2010
 *      Author: bmonkey
 */

#include "MengerSponge.h"
#include "Materials.h"
#include <algorithm>

MengerSponge::MengerSponge(unsigned recursion) {

	makeCube();
	makeSponge(recursion,{0,0,0});


}

MengerSponge::~MengerSponge() {
	// TODO Auto-generated destructor stub
}

void MengerSponge::makeSponge(unsigned recursion,vector<float> position){

	if (recursion == 0){
		Material * material = new MultiTextureMaterial();
		SceneGraph::Instance().addNode(new Node("", {0,0,-5}, 0.1, mesh,material));
		SceneGraph::Instance().addNode(new Node("", {-2,0,-5}, 3.0, mesh, material));
		SceneGraph::Instance().addNode(new Node("", {2,0,-5}, 1.0, mesh, material));
	}else{
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);

		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);

		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
		makeSponge(recursion--, position);
	}


}

void MengerSponge::makeCube(){
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

	vector<GLuint> indices;

	vector<GLuint> back = {
									//Back
									4, 0, 3,
									4, 3, 7,
	};
	indices.insert(indices.end(), back.begin(), back.end());

	vector<GLuint> front = {
									//Front
									1,5,2,
									5,6,2,
	};
	indices.insert(indices.end(), front.begin(), front.end());

	vector<GLuint> left = {
									//Left
									2, 6, 7,
									2, 7, 3,
	};
	indices.insert(indices.end(), left.begin(), left.end());

	vector<GLuint> right = {
									//Right
									0,4,1,
									4,5,1,
	};
	indices.insert(indices.end(), right.begin(), right.end());

	vector<GLuint> top = {
									//Top
									4,7,5,
									7,6,5,
	};
	indices.insert(indices.end(), top.begin(), top.end());

	vector<GLuint> bottom = {
									//Bottom
									0,1,2,
									0,2,3
	};
	indices.insert(indices.end(), bottom.begin(), bottom.end());

	mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    //mesh->addBuffer(color,3,"in_Color");
    mesh->addBuffer(vertices,3,"in_Normal");
    mesh->addBuffer(uvCoords,2,"in_Uv");
    mesh->addElementBuffer(indices);
    mesh->setDrawType(GL_TRIANGLES);
}

Mesh * MengerSponge::getMesh(){
	return mesh;
}
