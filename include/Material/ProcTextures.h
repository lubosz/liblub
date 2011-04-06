/*
 * ProcTextures.h
 *
 *  Created on: Apr 6, 2011
 *      Author: bmonkey
 */

#ifndef PROCTEXTURES_H_
#define PROCTEXTURES_H_

class ProcTextures {
public:
  ProcTextures();
  virtual ~ProcTextures();

  static QImage * makeGradientPhun(QSize size) {
    QImage * image = new QImage(size, QImage::Format_ARGB32);
    for(int x = 0; x < image->width(); x++) {
      for(int y = 0; y < image->height(); y++) {
        QRgb color = qRgba(x, y,0, x+y);
        image->setPixel(x,y,color);
      }
    }

    return image;
  }

  static QImage * makeGlow(QSize size, float exposure, float radius) {

    QImage * image = new QImage(size, QImage::Format_ARGB32);

    for (int y = 0; y < size.height(); y++) {
      for (int x = 0; x < size.width(); x++) {
        float fX = ((size.width() - 1) * 0.5f - x) / (float) (size.width() - 1);
        float fY = ((size.height() - 1) * 0.5f - y) / (float) (size.height() - 1);
        float distance = std::max(0.0f, (float)sqrt(fX * fX + fY * fY) - radius);
        float intensity = exp(-exposure * distance);
        unsigned char color = (unsigned char) (intensity * 192 + 0.5f);
        image->setPixel(x,y,qRgba(255, 255,255, color));
      }
    }
    return image;
  }
};

#endif /* PROCTEXTURES_H_ */
