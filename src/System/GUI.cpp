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

QImage * GUI::drawText(const string & text) {
  QSize textSize(350,350);
  QRectF textBox(0,0,textSize.width(), textSize.height());

  //clean image
  QImage * image = new QImage(textSize, QImage::Format_ARGB32);
  QPainter cleanPainter(image);
  cleanPainter.fillRect ( textBox, Qt::white );
  cleanPainter.end();

  //clean alpha channel
  QImage black(textSize, QImage::Format_Mono);
  QPainter blackPainter(&black);
  blackPainter.fillRect ( textBox, Qt::black );
  blackPainter.end();
  image->setAlphaChannel(black);

  //draw font
  QPainter fontPainter(image);
  fontPainter.setRenderHint(QPainter::Antialiasing, true);
  fontPainter.setRenderHint(QPainter::TextAntialiasing, true);
  fontPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  fontPainter.setFont(QFont("Arial", 20));
  fontPainter.setPen(Qt::white);
  fontPainter.drawText(textBox, Qt::AlignCenter, QString::fromStdString(text));
  fontPainter.end();

  return image;
}
