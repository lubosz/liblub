/*
 * Camera.h
 *
 *  Created on: Apr 1, 2010
 *      Author: bmonkey
 */

#pragma once

#include <QMatrix4x4>
#include "common.h"
#include "Singleton.h"

class Camera: public Singleton<Camera>{
public:

	void move(GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat yaw, GLfloat pitch, GLfloat roll);

	GLfloat x, y, z, aspect, fov, nearz, farz, yaw, pitch, roll;

	void setAspect(GLfloat aspect);
	void setParams(GLfloat fov, GLfloat nearz, GLfloat farz);
	void identity();
    void perspective();
    void update();
    QMatrix4x4 getView() const;
	QMatrix4x4 getProjection() const;
private:
    friend class Singleton<Camera>;
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;

    Camera();
    ~Camera();

};
