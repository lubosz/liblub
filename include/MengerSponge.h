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
	virtual ~MengerSponge();
	Mesh * getMesh();
};
