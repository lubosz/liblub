/*
 * Copyright © 2010 Lubosz Sarnecki
 * Input.h
 *
 *  Created on: Sep 28, 2010
 */

#pragma once

#include <qobject.h>

class Input : public QObject {
  Q_OBJECT
 public:
  float inputSpeed, mouseSensitivity;
	virtual void eventLoop() = 0;
};
