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
	Light(const QVector4D& position, const QVector3D & direction);
	virtual ~Light();
    QVector3D getDirection() const;
    QVector4D getPosition() const;
    void setColor(QVector4D & color);
    void setPosition(QVector4D& position);
    void bindShaderUpdate(ShaderProgram * shaderProgram);
    void bindShaderInit(ShaderProgram * shaderProgram);
    void bindShaderUpdateLight(ShaderProgram * shaderProgram);
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void moveForward();
    void moveBack();

    void update();

    QMatrix4x4 getView() const;
    QMatrix4x4 getProjection() const;
    QMatrix4x4 getViewNoTranslation() const;


    QMatrix4x4 viewMatrix, projectionMatrix;

	QVector3D direction;
	QVector4D position;
private:

	QVector4D color;

	float moveSensitivity;
};

