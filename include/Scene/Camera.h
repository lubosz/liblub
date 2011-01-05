/*
 * Copyright © 2010 Lubosz Sarnecki
 * Camera.h
 *
 *  Created on: Apr 1, 2010
 */

#pragma once

#include "common/Singleton.h"
#include "Scene/DirectionNode.h"

class Camera: public Singleton<Camera>, public DirectionNode {
 public:

	void updateRotation();

	void forward();
	void backward();
	void left();
	void right();

	void setMouseLook(int mouseXrel, int mouseYrel);
	void setMouseZoom(int wheelX, int wheelY);
 private:

    friend class Singleton<Camera>;

    Camera();
    ~Camera();
};
