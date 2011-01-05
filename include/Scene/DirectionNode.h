/*
 * Copyright © 2010 Lubosz Sarnecki
 * DirectionNode.h
 *
 *  Created on: Oct 5, 2010
 */

#pragma once

#include "common/Qt3D.h"

const QVector3D up(0, 1, 0);

class DirectionNode {
 public:
	qreal aspect, fov, near, far,
		yaw, pitch, roll;
	QVector3D position, direction, defaultCenter;

    QMatrix4x4 viewMatrix, projectionMatrix, rotation;

	float mouseSensitivity, speed;

	QVector3D getDirection() const;
	QVector3D getPosition() const;

    QMatrix4x4 getView() const;
    QMatrix4x4 getViewNoTranslation() const;
	QMatrix4x4 getProjection() const;

    void setPosition(const QVector3D& position);
	void setAspect(qreal aspect);
	void setParams(qreal fov, qreal near, qreal far);

    void updatePerspective();
    void updateView();

    void defaultValues();
};
