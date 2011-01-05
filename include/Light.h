/*
 * Copyright © 2010 Lubosz Sarnecki
 * Light.h
 *
 *  Created on: Oct 1, 2010
 */

#pragma once

#include "ShaderProgram.h"
#include "DirectionNode.h"

class Light : public DirectionNode {
 public:
	Light(const QVector3D& position, const QVector3D & direction);
	virtual ~Light();
    QVector3D getDirection() const;
    QVector3D getPosition() const;
    void setColor(QVector4D & color);

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

 private:
	QVector4D color;
};

