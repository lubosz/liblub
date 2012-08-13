/*
 * Copyright © 2010 Lubosz Sarnecki
 * Input.h
 *
 *  Created on: Sep 28, 2010
 */

#pragma once

#include "Window/Input.h"
#include "common/Singleton.h"
#include <QTimer>
#include <QVector2D>

class QtInput: public Input, public Singleton<QtInput>{
    Q_OBJECT
public:
  QtInput();
  virtual ~QtInput();
  QTimer *inputTimer;
  QList<int> pressedKeys;
  float inputSpeed;
  QVector2D mousePosition;

  void eventLoop() {};

signals:
  void draw();
public slots:
  void move();
};
