/*
 * MengerSponge.cpp
 *
 *  Created on: Jul 11, 2010
 *      Author: bmonkey
 */

#include "MengerSponge.h"

#include <algorithm>

MengerSponge::MengerSponge(unsigned recursion) {

	//material = new StarMat();
	//material = new BrickMaterial();
	//material = new NormalColorMat();
	material = new PhongColorMat(QVector3D(1,1,1), "bunny.png");
	//material = new TextureMaterial("Paper_Texture_by_Spiteful_Pie_Stock.jpg");
	//makeCube();
	makeSponge(recursion,{0,0,0}, 1.0f);
/*
	cout << "Vector size:"<< oneMeshVertices.size() << "\n";
	cout << "Removing doubles...";
	sort(oneMeshVertices.begin(), oneMeshVertices.end());
	oneMeshVertices.erase(unique(oneMeshVertices.begin(), oneMeshVertices.end()), oneMeshVertices.end());
	cout << "Vector size:"<< oneMeshVertices.size() << "\n";



	for (int i = 0; i < oneMeshVertices.size(); i++){
		oneMeshIndices.push_back(i);
	}
*/
	mesh = new Mesh();
    mesh->addBuffer(oneMeshVertices,3,"in_Vertex");
    mesh->addBuffer(oneMeshVertices,3,"in_Normal");

    //mesh->addBuffer(oneMeshNormals,3,"in_Normal");

    mesh->addBuffer(oneMeshVertices,3,"in_Color");


    //mesh->addBuffer(oneMeshUVCoords,2,"in_Uv");
    mesh->addElementBuffer(oneMeshIndices);
    mesh->setDrawType(GL_TRIANGLES);
    SceneGraph::Instance().addNode(new Node("", QVector3D(0,0,-10), 10.0, mesh,material));


}

MengerSponge::~MengerSponge() {
	// TODO Auto-generated destructor stub
}

void MengerSponge::makeSponge(unsigned recursion,const vector<float> & position, float size){

	if (recursion == 0){

		//SceneGraph::Instance().addNode(new Node("", position, size, mesh,material));
		addCube(position, size);
	}else{
		recursion-=1;
		float trans = size*2/3.0f;

		//TOP
		makeSponge(recursion, {position[0],position[1]+trans,position[2]+trans}, size/3.0);

		makeSponge(recursion, {position[0]-trans,position[1]+trans,position[2]}, size/3.0);
		makeSponge(recursion, {position[0],position[1]+trans,position[2]-trans}, size/3.0);
		makeSponge(recursion, {position[0]+trans,position[1]+trans,position[2]}, size/3.0);


		makeSponge(recursion, {position[0]+trans,position[1]+trans,position[2]+trans}, size/3.0);
		makeSponge(recursion, {position[0]+trans,position[1]+trans,position[2]-trans}, size/3.0);

		makeSponge(recursion, {position[0]-trans,position[1]+trans,position[2]+trans}, size/3.0);
		makeSponge(recursion, {position[0]-trans,position[1]+trans,position[2]-trans}, size/3.0);

		//MIDDLE
		makeSponge(recursion, {position[0]+trans,position[1],position[2]+trans}, size/3.0);
		makeSponge(recursion, {position[0]+trans,position[1],position[2]-trans}, size/3.0);

		makeSponge(recursion, {position[0]-trans,position[1],position[2]+trans}, size/3.0);
		makeSponge(recursion, {position[0]-trans,position[1],position[2]-trans}, size/3.0);

		//BOTTOM
		makeSponge(recursion, {position[0],position[1]-trans,position[2]+trans}, size/3.0);
		makeSponge(recursion, {position[0],position[1]-trans,position[2]-trans}, size/3.0);

		makeSponge(recursion, {position[0]+trans,position[1]-trans,position[2]}, size/3.0);
		makeSponge(recursion, {position[0]-trans,position[1]-trans,position[2]}, size/3.0);

		makeSponge(recursion, {position[0]+trans,position[1]-trans,position[2]+trans}, size/3.0);
		makeSponge(recursion, {position[0]+trans,position[1]-trans,position[2]-trans}, size/3.0);

		makeSponge(recursion, {position[0]-trans,position[1]-trans,position[2]+trans}, size/3.0);
		makeSponge(recursion, {position[0]-trans,position[1]-trans,position[2]-trans}, size/3.0);

	}


}

void MengerSponge::addCube(const vector<float> & position, float size){

	unsigned indexSize = oneMeshVertices.size()/3;

	vector<GLfloat> vertices = {
			size + position[0], -size + position[1], -size + position[2],
			size + position[0], -size + position[1], size + position[2],
			-size + position[0], -size + position[1], size + position[2],
			-size + position[0], -size + position[1], -size + position[2],
			size + position[0], size + position[1], -size + position[2],
			size + position[0], size + position[1], size + position[2],
			-size + position[0], size + position[1], size + position[2],
			-size + position[0], size + position[1], -size + position[2]
	};

	oneMeshVertices.insert(oneMeshVertices.end(), vertices.begin(), vertices.end());

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

	oneMeshUVCoords.insert(oneMeshUVCoords.end(), uvCoords.begin(), uvCoords.end());

	vector<GLfloat> normals = {
			1.0, -1.0, -1.0,
			1.0, -1.0, 1.0,
			-1.0, -1.0, 1.0,
			-1.0, -1.0, -1.0,
			1.0, 1.0, -1.0,
			1.0, 1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0
	};

	oneMeshNormals.insert(oneMeshNormals.end(), normals.begin(), normals.end());


	//cout << "IndexSize: " << indexSize <<"\n";

	vector<GLuint> indices;

	vector<GLuint> back = {
									//Back
									4+indexSize, indexSize, 3+indexSize,
									4+indexSize,  3+indexSize,  7+indexSize
	};

	indices.insert(indices.end(), back.begin(), back.end());

	vector<GLuint> front = {
									//Front
									1+indexSize, 5+indexSize, 2+indexSize,
									5+indexSize, 6+indexSize, 2+indexSize
	};
	indices.insert(indices.end(), front.begin(), front.end());

	vector<GLuint> left = {
									//Left
									2+indexSize,  6+indexSize,  7+indexSize,
									2+indexSize,  7+indexSize,  3+indexSize
	};
	indices.insert(indices.end(), left.begin(), left.end());

	vector<GLuint> right = {
									//Right
									indexSize, 4+indexSize, 1+indexSize,
									4+indexSize, 5+indexSize, 1+indexSize
	};
	indices.insert(indices.end(), right.begin(), right.end());

	vector<GLuint> top = {
									//Top
									4+indexSize, 7+indexSize, 5+indexSize,
									7+indexSize, 6+indexSize, 5+indexSize
	};
	indices.insert(indices.end(), top.begin(), top.end());

	vector<GLuint> bottom = {
									//Bottom
									indexSize, 1+indexSize, 2+indexSize,
									indexSize, 2+indexSize, 3+indexSize
	};
	indices.insert(indices.end(), bottom.begin(), bottom.end());

	oneMeshIndices.insert(oneMeshIndices.end(), indices.begin(), indices.end());

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
/*
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
*/
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
    //mesh->setDrawType(GL_LINES);
}
