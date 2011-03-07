/*
 * GUI.cpp
 *
 *  Created on: Mar 6, 2011
 *      Author: bmonkey
 */

#include "System/GUI.h"

#include <QSize>
#include <QRectF>
#include <QPainter>
#include <QDebug>

GUI::GUI() {
  // TODO Auto-generated constructor stub

}

GUI::~GUI() {
  // TODO Auto-generated destructor stub
}

QImage * GUI::drawText(const QString & text) {
  QSize textSize(200,20);
//  QImage image(textSize, QImage::Format_ARGB32_Premultiplied);
  QImage * image = new QImage(textSize, QImage::Format_ARGB32);
  QPainter painter(image);

  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

  painter.setBackground(Qt::white);

  painter.setFont(QFont("Arial", 30));
  QRectF textBox(0,0,textSize.width(), textSize.height());
  painter.drawText(textBox, Qt::AlignCenter, text);
  qDebug() << text;

  painter.end();
  return image;
//  image.save("foo.png");
}
