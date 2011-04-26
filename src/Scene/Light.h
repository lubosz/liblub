/*
 * Copyright © 2010 Lubosz Sarnecki
 * Light.h
 *
 *  Created on: Oct 1, 2010
 */

#pragma once

#include "Material/ShaderProgram.h"
#include "Scene/DirectionNode.h"

class Light : public DirectionNode {
 public:
  Light();
	Light(const QVector3D& position, const QVector3D & direction);
	virtual ~Light();
  QVector3D getDirection() const;
  QVector3D getPosition() const;

  void bindShaderUpdate(ShaderProgram * shaderProgram);
  void bindShaderInit(ShaderProgram * shaderProgram);
  void bindShaderUpdateLight(ShaderProgram * shaderProgram);

  void update();

  QVector4D diffuse, specular;
};

