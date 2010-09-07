/*
 * Camera.h
 *
 *  Created on: Apr 1, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "Singleton.h"

//TODO: Matrix class, dry
#define IDENTITY_MATRIX4 { 1.0, 0.0, 0.0, 0.0,\
                           0.0, 1.0, 0.0, 0.0,\
                           0.0, 0.0, 1.0, 0.0,\
                           0.0, 0.0, 0.0, 1.0 }


const GLfloat identitymatrix[16] = IDENTITY_MATRIX4;

class Camera: public Singleton<Camera>{
public:

	void move(GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat yaw, GLfloat pitch, GLfloat roll);
    GLfloat * getProjectionmatrix();
	GLfloat x, y, z, aspect, fov, nearz, farz, yaw, pitch, roll;

	void setAspect(GLfloat aspect);
	void setParams(GLfloat fov, GLfloat nearz, GLfloat farz);
	void identity();
	/* Generate a perspective view matrix using a field of view angle fov,
	 * window aspect ratio, near and far clipping planes */
	void perspective();

private:
	friend class Singleton<Camera>;


	GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */



private:
	Camera(); // Private constructor
	~Camera();

};
