/*
 * MengerSponge.h
 *
 *  Created on: Jul 11, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Mesh.h"

class MengerSponge {

private:
	Mesh * mesh;

public:
	MengerSponge(unsigned recursion);
	void makeSponge(unsigned recursion,vector<float> position);
	void makeCube();
	virtual ~MengerSponge();
	Mesh * getMesh();
};
