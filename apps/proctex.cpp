/*
    Copyright © 2010 Lubosz Sarnecki

    This file is part of liblub.

    liblub is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    liblub is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with liblub.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <string>
#include <QApplication>
#include "System/Application.h"
#include "System/GUI.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include <QPainter>
#include <math.h>

class LoadApp: public Application {
 public:


  explicit LoadApp(string sceneName) {
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~LoadApp() {}

  QImage * makeNoise() {
    QSize textSize(1000,1000);
    QRectF textBox(0,0,textSize.width(), textSize.height());

    //clean image
    QImage * image = new QImage(textSize, QImage::Format_ARGB32);
    QPainter cleanPainter(image);
    cleanPainter.fillRect ( textBox, Qt::white );
    cleanPainter.end();

    for(int x = 0; x < image->width(); x++) {
      for(int y = 0; y < image->height(); y++) {
        QRgb color = qRgba(x, y,0, x+y);
        image->setPixel(x,y,color);
//    image->pixel();

      }
    }

    return image;
  }

  QImage * makeGlow(QSize size, float exposure, float radius) {

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

  void scene() {
    sceneLoader->load();

//    QImage * image = makeNoise();
    QImage * image = makeGlow(QSize(1000,1000),40.0f, 0.1f);

    Texture * textTexture = TextureFactory::Instance().load(image,"myTexture");

    Material * material = new EmptyMat();
    material->shaderProgram = SceneData::Instance().shaderPrograms.value("Texture");
    material->addTexture(textTexture);
    Node * plane = new Node("Plane", { 0,0,-2 }, 1, MeshFactory::load(
        "plane.blend"), material);
    plane->transparent = true;
    plane->setRotation(QVector3D(-90,0,180));
    SceneGraph::Instance().addNode(plane);
    GUI::Instance().init();
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  LoadApp("proctex").run();
}

