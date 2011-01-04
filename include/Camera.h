/*
 * Camera.h
 *
 *  Created on: Apr 1, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Singleton.h"
#include "DirectionNode.h"

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
