/*
 * GUI.cpp
 *
 *  Created on: Mar 6, 2011
 *      Author: bmonkey
 */

#include "System/GUI.h"

#include <sstream>
#include <string>
#include <QSize>
#include <QRectF>
#include <QDebug>
#include <QMap>

#include "Material/Materials.h"
#include "Scene/SceneData.h"
#include "Mesh/Geometry.h"
#include "Window/MediaLayer.h"
#include "System/Logger.h"

GUI::GUI() {
  textLines = QMap<string,string>();
  material = new Simple("Texture/font");

  node = new Node("GUI", { 0, 0, 0 }, 1, Geometry::makePlane(QRectF(0.5,0,0.5,1)), material);
//      plane->transparent = true;
//      plane->setRotation(QVector3D(-90,0,180));
//      plane->setName("font");



  //    Material * material = new PhongColor(QVector3D(1, 1, 1));
  //    Node * plane = new Node("Plane", { 0, 0, 0 }, 2, MeshFactory::load(
  //        "plane.blend"), material);
  screenSize = QSize(480,600);
  textBox = QRectF(0,0,screenSize.width(), screenSize.height());
  image = new QImage(screenSize, QImage::Format_ARGB32);

  //clean alpha channel
  black = QImage(screenSize, QImage::Format_Mono);
  QPainter blackPainter(&black);
  blackPainter.fillRect ( textBox, Qt::black );
  blackPainter.end();


  clear();
  fontPainter = new QPainter(image);
   drawBox = QRectF(50,200,screenSize.width()-100, screenSize.height()-100);
//  fontPainter->end();
}

GUI::~GUI() {
  // TODO Auto-generated destructor stub
}
void GUI::init() {
  GUI::Instance().addText("ascene", SceneData::Instance().name);
  GUI::Instance().addText("fps", "FPS");
//  GUI::Instance().addText("zoom", "Zoom");
  GUI::Instance().addText("cam", "Cam");
  render();
  texture = TextureFactory::Instance().load(image,"myTexture");
  material->addTexture(texture);
}

void GUI::update() {
  std::stringstream fps, cam,zoom;
//    windowTitle << programTile << " - FPS: " << fps_current;
  fps << "FPS: " << MediaLayer::Instance().fps_current;
  updateText("fps",fps.str());
  cam << "\nCam:\n" << SceneData::Instance().getCurrentCamera()->position.x()
      <<"\n " << SceneData::Instance().getCurrentCamera()->position.y()
      <<"\n " << SceneData::Instance().getCurrentCamera()->position.z();
//  zoom << "\nZoom:\n";
  updateText("fps",fps.str());
  updateText("cam",cam.str());
//  updateText("zoom",zoom.str());
  clear();
  fontPainter->begin(image);
  render();
  glBindTexture(GL_TEXTURE_2D, texture->texture);
  texture->loadQImage(GL_TEXTURE_2D,image);
}

void GUI::clear() {
  image->setAlphaChannel(black);
  //clean image
//  QPainter cleanPainter(image);
//  fontPainter->fillRect ( textBox, Qt::white );
//  fontPainter->end();
//
//
//  image->setAlphaChannel(black);
}

void GUI::render() {
  //draw font
//  fontPainter->begin(image);
   fontPainter->setRenderHint(QPainter::Antialiasing, true);
    fontPainter->setRenderHint(QPainter::TextAntialiasing, true);
    fontPainter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    fontPainter->setFont(QFont("Ubuntu", 24));
    fontPainter->setLayoutDirection(Qt::RightToLeft);
    fontPainter->setPen(Qt::white);



//  fontPainter->drawRect(drawBox);

  string text;
  foreach(string line, textLines) {
    text += line +"\n";
  }

  fontPainter->drawText(drawBox, Qt::AlignLeft, QString::fromStdString(text));
  fontPainter->end();

}

void GUI::addText(string id, string value){
  textLines.insert(id, value);
}
void GUI::updateText(string id, string value) {
  textLines[id] = value;
}

void GUI::draw() {
  glError;
  glEnable(GL_BLEND);
  ShaderProgram * shaderProgram = material->getShaderProgram();
  shaderProgram->use();
  shaderProgram->setUniform(QMatrix4x4(), "MVPMatrix");
  node->draw();
  glDisable(GL_BLEND);
  glError;
}
