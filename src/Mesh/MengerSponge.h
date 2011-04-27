/*
 * Copyright © 2010 Lubosz Sarnecki
 * MengerSponge.h
 *
 *  Created on: Jul 11, 2010
 */

#pragma once

#include "Mesh/Mesh.h"
#include "Material/Materials.h"

class MengerSponge {
 private:
	Mesh * mesh;

	void makeSponge(
	        unsigned recursion,
	        const vector<float> & position,
	        float size
	);
	void addCube(const vector<float> & position, float size);

 public:
	explicit MengerSponge(const QList<string> & attributes, unsigned recursion);
	virtual ~MengerSponge();

	Mesh * getMesh();
};
