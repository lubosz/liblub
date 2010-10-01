/*
 * Light.h
 *
 *  Created on: Oct 1, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "QVector3D"
#include "QVector4D"
#include "ShaderProgram.h"

class Light {
public:
	Light(QVector4D position);
	virtual ~Light();
    QVector3D getDirection() const;
    QVector4D getPosition() const;
    void setColor(QVector4D & color);
    void setDirection(QVector3D direction);
    void setPosition(QVector4D& position);
    void update();
    void bindShader(ShaderProgram * shaderProgram);
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void moveForward();
    void moveBack();

private:
	QVector3D direction;
	QVector4D position;
	QVector4D color;

	float moveSensitivity;
};

