/*
 * Face.h
 *
 *  Created on: Jun 10, 2010
 *      Author: bmonkey
 */

#pragma once

class Face {
public:

	unsigned vertIndices[3], normalIndices[3], uvIndices[3];

	Face(unsigned vertex1, unsigned vertex2, unsigned vertex3);
	void setNormalIndices(unsigned normal1, unsigned normal2, unsigned normal3);
	void setUvIndices(unsigned uv1, unsigned uv2, unsigned uv3);
	virtual ~Face();
};

