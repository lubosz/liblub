/*
 * Camera.h
 *
 *  Created on: Apr 1, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "Singleton.h"

class Camera: public Singleton<Camera>{
public:

	void move(GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat yaw, GLfloat pitch, GLfloat roll);
    GLfloat * getProjectionmatrix();
	GLfloat x, y, z, aspect, fov, nearz, farz, yaw, pitch, roll;

	void setAspect(GLfloat aspect);
	void setParams(GLfloat fov, GLfloat nearz, GLfloat farz);

private:
	friend class Singleton<Camera>;


	GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */

	/* Generate a perspective view matrix using a field of view angle fov,
	 * window aspect ratio, near and far clipping planes */
	void perspective();

private:
	Camera(); // Private constructor
	~Camera();

};
