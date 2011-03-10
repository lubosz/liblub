/*
 * Copyright © 2010 Lubosz Sarnecki
 * Camera.h
 *
 *  Created on: Apr 1, 2010
 */

#pragma once

#include "common/Singleton.h"
#include "Scene/DirectionNode.h"

class Camera: public DirectionNode {
 public:
    Camera();
    ~Camera();

	void updateRotation();
	void update();
	void setMouseLook(int mouseXrel, int mouseYrel, qreal mouseSensitivity);
	void setMouseZoom(int wheelX, int wheelY);
};
