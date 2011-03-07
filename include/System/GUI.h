/*
 * GUI.h
 *
 *  Created on: Mar 6, 2011
 *      Author: bmonkey
 */

#pragma once

#include <QString>
#include <QImage>

class GUI {
public:
  GUI();
  virtual ~GUI();
  QImage * drawText(const QString & text);
};
