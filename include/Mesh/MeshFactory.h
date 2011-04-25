/*
 * Copyright © 2010 Lubosz Sarnecki
 * MeshFactory.h
 *
 *  Created on: Jun 8, 2010
 */

#pragma once

#include "Mesh.h"

class MeshFactory {
 public:
	static Mesh * load(string file);
};

