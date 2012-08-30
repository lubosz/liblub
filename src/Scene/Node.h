/*
 * Copyright © 2010 Lubosz Sarnecki
 * Node.h
 *
 *  Created on: Jun 28, 2010
 */

#pragma once

#include "Mesh/Mesh.h"
#include "Scene/Material.h"
#include <QObject>
#include <QMatrix4x4>

class DirectionNode;
class ShaderProgram;

class Node: public QObject {

  friend class TreeSponge;
  Q_OBJECT
  Q_PROPERTY(float size READ size WRITE setSize)
  Q_PROPERTY(float rotationX READ rotationX WRITE setRotationX)
  Q_PROPERTY(float rotationY READ rotationY WRITE setRotationY)
  Q_PROPERTY(float rotationZ READ rotationZ WRITE setRotationZ)
  Q_PROPERTY(float positionX READ positionX WRITE setPositionX)
  Q_PROPERTY(float positionY READ positionY WRITE setPositionY)
  Q_PROPERTY(float positionZ READ positionZ WRITE setPositionZ)

private:
  bool castShadows, receiveShadows;
  float m_size;
  string name;
  QVector3D eulerRotationCache;
  Node * parent;
public:
  QMatrix4x4 modelMatrix;
  Material * material;
  ShaderProgram * shader;
  QVector3D position;
  QQuaternion rotation;
  Mesh * mesh;
  QMatrix4x4 rotationMatrix;

  Node();

  Node(string name, const QVector3D& position, float size, Mesh * mesh,
      ShaderProgram * shaderProgram);

  Node(string name, const QVector3D& position, float size, Mesh * mesh,
      Material * material);

  virtual ~Node();

  void draw();
  virtual void draw(ShaderProgram * shaderProgram);
//  void draw(ShaderProgram * shader, DirectionNode * camView);
  void update();
  void updateRotationFromEuler();

  bool hasShader() const;

  float size() const;
  float rotationX() const;
  float rotationY() const;
  float rotationZ() const;
  float positionX() const;
  float positionY() const;
  float positionZ() const;
  bool getCastShadows() const;
  bool getReceiveShadows() const;
  string getName() const;
  QVector3D getPosition() const;
  ShaderProgram *getShader() const;
  QVector3D getCenter();
  QVector3D direction();

  void setTransparent(bool transparent);
  bool getTransparent();

  Material * getMaterial() const;

  Mesh * getMesh();
  void setMesh(Mesh * mesh);


  void setRotation(const QMatrix4x4& rotationMatrix);
  void setRotation(const QQuaternion& rotationQuat);
  void setDirection(const QVector3D & direction);
  void setCastShadows(bool castShadows);
  void setReceiveShadows(bool receiveShadows);
  void setPosition(const QVector3D& position);
  void setRotation(const QVector3D& rotationVec);
  void setView(DirectionNode * viewPoint);
  void setView(ShaderProgram * shaderProgram, DirectionNode * viewPoint);
  void setShadowCoords(DirectionNode * viewPoint);
  void setName(string name);
  void setMaterial(Material *material);
  static void setShadowCoords(ShaderProgram * shaderProgram,
      DirectionNode * viewPoint);
  void setShader(ShaderProgram* shader);

public slots:
  void setSize(float size);
  void setRotationX(float rotationX);
  void setRotationY(float rotationY);
  void setRotationZ(float rotationZ);
  void setPositionX(float positionX);
  void setPositionY(float positionY);
  void setPositionZ(float positionZ);
};

