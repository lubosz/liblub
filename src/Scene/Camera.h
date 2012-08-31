/*
 * Copyright © 2010 Lubosz Sarnecki
 * Camera.h
 *
 *  Created on: Apr 1, 2010
 */

#pragma once

#include "Common/Singleton.h"
#include "Scene/DirectionNode.h"
#include "Shader/ShaderProgram.h"

class Camera: public DirectionNode {
 public:
    Camera();
    ~Camera();
	void update();
	void setMouseLook(int mouseXrel, int mouseYrel, qreal mouseSensitivity);
	void setMouseZoom(int wheelX, int wheelY);
	void setUniforms(ShaderProgram * program, const QVector3D fromPosition);
};
