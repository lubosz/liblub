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

const QVector3D up = {0,1,0};

class Camera: public Singleton<Camera>{
public:

	void move(GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat yaw, GLfloat pitch, GLfloat roll);

	GLfloat aspect, fov, nearz, farz, yaw, pitch, roll;

	void setAspect(GLfloat aspect);
	void setParams(GLfloat fov, GLfloat nearz, GLfloat farz);
	void identity();
    void perspective();
    void update();
    QMatrix4x4 getView() const;
	QMatrix4x4 getProjection() const;
	QVector3D center, eye;

	void forward();
	void backward();
	void left();
	void right();



	void setMouseLook(int mouseXrel, int mouseYrel);
private:
	float mouseSensitivity;

    friend class Singleton<Camera>;
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;
	float speed;

    Camera();
    ~Camera();

};
