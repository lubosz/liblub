/*
 * Copyright © 2010 Lubosz Sarnecki
 * MeshFactory.h
 *
 *  Created on: Jun 8, 2010
 */

#pragma once

#include "Mesh.h"
#include <assimp/scene.h>

class MeshLoader {
 public:
    static Mesh * getMeshFromAssimp(aiMesh * assMesh, const QList<string> & attributes, bool changePositionOrder);
	static Mesh * load(const QList<string> & attributes, string file);
};

