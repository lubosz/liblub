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

public:
	MengerSponge(unsigned recursion);
	void makeSponge(unsigned recursion,vector<float> position, float size);
	void makeCube();
	virtual ~MengerSponge();
	Mesh * getMesh();
};
