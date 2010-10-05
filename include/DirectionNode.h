/*
 * DirectionNode.h
 *
 *  Created on: Oct 5, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "QVector3D"
#include "QMatrix4x4"

const QVector3D up(0,1,0);

class DirectionNode {
public:
	GLfloat aspect, fov, near, far,
		yaw, pitch, roll;
	QVector3D position, direction, defaultCenter;

    QMatrix4x4 viewMatrix, projectionMatrix, rotation;

	float mouseSensitivity, speed;

	QVector3D getDirection() const;
	QVector3D getPosition() const;

    QMatrix4x4 getView() const;
    QMatrix4x4 getViewNoTranslation() const;
	QMatrix4x4 getProjection() const;

    void setPosition(QVector3D& position);
	void setAspect(GLfloat aspect);
	void setParams(GLfloat fov, GLfloat near, GLfloat far);

    void updatePerspective();
    void updateView();

    void defaultValues();

};
