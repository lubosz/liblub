/*
 * Face.cpp
 *
 *  Created on: Jun 10, 2010
 *      Author: bmonkey
 */

#include "Face.h"

Face::Face(unsigned vertex1, unsigned vertex2, unsigned vertex3)
{
	vertIndices[0] = vertex1;
	vertIndices[1] = vertex2;
	vertIndices[2] = vertex3;
}

Face::~Face() {
	// TODO Auto-generated destructor stub
}

void Face::setNormalIndices(unsigned normal1, unsigned normal2, unsigned normal3){
	normalIndices[0] = normal1;
	normalIndices[1] = normal2;
	normalIndices[2] = normal3;
}
void Face::setUvIndices(unsigned uv1, unsigned uv2, unsigned uv3){
	uvIndices[0] = uv1;
	uvIndices[1] = uv2;
	uvIndices[2] = uv3;
}
