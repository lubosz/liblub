/*
 * Camera.h
 *
 *  Created on: Apr 1, 2010
 *      Author: bmonkey
 */

#pragma once

/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

class Camera {
public:

	GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */
	GLfloat x, y, z, aspect, fov, nearz, farz, yaw, pitch, roll;


	static Camera& Instance()
	 {
	    static Camera singleton;
	    return singleton;
	  }

	/* Generate a perspective view matrix using a field of view angle fov,
	 * window aspect ratio, near and far clipping planes */
	void perspective();

	void move(GLfloat x, GLfloat y, GLfloat z);

	void setAspect(GLfloat aspect);

	void setParams(GLfloat fov, GLfloat nearz, GLfloat farz);

	void rotate(GLfloat yaw, GLfloat pitch, GLfloat roll);




private:
	Camera(); // Private constructor
	~Camera();
	Camera(const Camera&); // Prevent copy-construction
	Camera& operator=(const Camera&); // Prevent assignment

};
