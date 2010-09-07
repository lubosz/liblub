/*
 * MengerSponge.h
 *
 *  Created on: Jul 11, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Mesh.h"
#include "Materials.h"

class MengerSponge {

private:
	Mesh * mesh;
	Material * material;

	vector<GLfloat> oneMeshVertices;
	vector<GLfloat> oneMeshUVCoords;
	vector<GLuint> oneMeshIndices;
	vector<GLfloat> oneMeshNormals;

	void makeSponge(unsigned recursion,const vector<float> & position, float size);
	void makeCube();
	void addCube(const vector<float> & position, float size);

public:
	MengerSponge(unsigned recursion);
	virtual ~MengerSponge();
};