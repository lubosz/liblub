/*
 * GUI.cpp
 *
 *  Created on: Mar 6, 2011
 *      Author: bmonkey
 */

#include "System/FontOverlay.h"

#include <sstream>
#include <string>
#include <QSize>
#include <QRectF>
#include <QDebug>
#include <QMap>

#include "Material/Textures.h"
#include "Scene/Scene.h"
#include "Mesh/Geometry.h"
#include "Window/Window.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "Material/Shaders.h"

FontOverlay::FontOverlay() {
  textLines = QMap<string,string>();
  QList<string> attributes = QList<string>();
  attributes.push_back("uv");
  shader = new SimpleProgram("Texture/font",attributes);
  mesh = Geometry::plane(attributes, QRectF(0.5,0,0.5,1));
  attributes.clear();
//  attributes.~QList();

  screenSize = QSize(480,600);
  textBox = QRectF(0,0,screenSize.width(), screenSize.height());
  image = QImage(screenSize, QImage::Format_ARGB32);

  //clean alpha channel
  black = QImage(screenSize, QImage::Format_Mono);
  QPainter blackPainter(&black);
  blackPainter.fillRect ( textBox, Qt::black );
  blackPainter.end();

  clear();
  drawBox = QRectF(50,200,screenSize.width()-100, screenSize.height()-100);

  texture = nullptr;
}

FontOverlay::~FontOverlay() {
    delete mesh;
    delete shader;
    if (texture != nullptr)
        delete texture;
}
void FontOverlay::init() {
  addText("ascene", Scene::Instance().name);
  addText("fps", "FPS");
  addText("cam", "Cam");
  addText("time", "Time");
  render();
  texture = new TextureQImage(image,"myTexture");
  shader->addTexture(texture);
  shader->samplerUniforms();
}

void FontOverlay::update() {
  std::stringstream fps, cam, zoom, time;
//  fps << "FPS: " << Timer::Instance().getFPS();
//  time << "ms: " << Timer::Instance().getSPF();
  updateText("fps",fps.str());
  cam << "\nCam:\n" << Scene::Instance().getCurrentCamera()->position.x()
      <<"\n " << Scene::Instance().getCurrentCamera()->position.y()
      <<"\n " << Scene::Instance().getCurrentCamera()->position.z();
  updateText("fps",fps.str());
  updateText("cam",cam.str());
  updateText("time",time.str().substr (0,12));
  clear();
  render();
  texture->bind();
  texture->loadQImage(image);
}

void FontOverlay::clear() {
  image.setAlphaChannel(black);
}

void FontOverlay::render() {
  QPainter fontPainter(&image);
  //draw font
  fontPainter.setRenderHint(QPainter::Antialiasing, true);
  fontPainter.setRenderHint(QPainter::TextAntialiasing, true);
  fontPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  fontPainter.setFont(QFont("Ubuntu", 24));
  fontPainter.setLayoutDirection(Qt::RightToLeft);
  fontPainter.setPen(Qt::white);

  string text = "";
  foreach(string line, textLines) {
    text += line + "\n";
  }

  fontPainter.drawText(drawBox, Qt::AlignLeft, QString::fromStdString(text));
  fontPainter.end();

}

void FontOverlay::addText(string id, string value){
  textLines.insert(id, value);
}
void FontOverlay::updateText(string id, string value) {
  textLines[id] = value;
}

void FontOverlay::draw() {
  glError;
  glEnable(GL_BLEND);
  shader->use();
  shader->setUniform("MVPMatrix",QMatrix4x4());
  shader->activateAndBindTextures();
  mesh->draw();
  glDisable(GL_BLEND);
  glError;
}
