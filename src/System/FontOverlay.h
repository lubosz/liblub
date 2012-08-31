/*
 * GUI.h
 *
 *  Created on: Mar 6, 2011
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include <QImage>
#include "Common/Singleton.h"
#include "Scene/Node.h"
#include "Texture/Texture.h"
#include <QMap>
#include <QPainter>
using std::string;

class FontOverlay {
public:
  QMap<string,string> textLines;
  QImage image;
  ShaderProgram * shader;
  Mesh * mesh;
  QRectF textBox;
  QSize screenSize;
  QImage black;
  Texture *texture;
  QRectF drawBox;
  FontOverlay();
  virtual ~FontOverlay();
  void addText(string id, string value);
  void updateText(string id, string value);
  void render();
  void draw();
  void init();
  void clear();
  void update();
};
