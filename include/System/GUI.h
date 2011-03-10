/*
 * GUI.h
 *
 *  Created on: Mar 6, 2011
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include <QImage>

using std::string;

class GUI {
public:
  GUI();
  virtual ~GUI();
  QImage * drawText(const string & text);
};
