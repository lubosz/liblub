/*
 * Copyright © 2010 Lubosz Sarnecki
 * Node.h
 *
 *  Created on: Jun 28, 2010
 */

#pragma once

#include "Mesh/Mesh.h"
#include "Material/Material.h"
#include <QObject>
#include <QVariant>
#include <QMetaType>

class DirectionNode;

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
  QMatrix4x4 modelMatrix;
  Node * parent;

public:
  Material * material;
  bool transparent;
  QVector3D position;
  QMatrix4x4 rotationMatrix;
  Mesh * mesh;

  Node() {
      this->material = nullptr;
      position = QVector3D();
      m_size = 1.0f;
      mesh = nullptr;
  } ;

  Node(string name, const QVector3D& position, float size, Mesh * mesh,
      Material * material);

  virtual ~Node();

  virtual void draw();
  virtual void draw(Material * material);
  void update();
  void updateRotationFromEuler();

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
  Material *getMaterial() const;
  QVector3D getCenter();
  QVector3D direction();

  void setDirection(const QVector3D & direction);
  void setCastShadows(bool castShadows);
  void setReceiveShadows(bool receiveShadows);
  void setMesh(Mesh *mesh);
  void setPosition(const QVector3D& position);
  void setRotation(const QVector3D& rotation);
  void setView(DirectionNode * viewPoint);
  void setView(ShaderProgram * shaderProgram, DirectionNode * viewPoint);
  void setShadowCoords(DirectionNode * viewPoint);
  void setName(string name);
  void setMaterial(Material *material);
  static void setShadowCoords(ShaderProgram * shaderProgram,
      DirectionNode * viewPoint);

public slots:
  void setSize(float size);
  void setRotationX(float rotation);
  void setRotationY(float rotation);
  void setRotationZ(float rotation);
  void setPositionX(float position);
  void setPositionY(float position);
  void setPositionZ(float position);
};

