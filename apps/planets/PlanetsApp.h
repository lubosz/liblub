/*
 * PlanetsApp.h
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#ifndef PLANETSAPP_H_
#define PLANETSAPP_H_

#include <QApplication>
#include <QtGui>

#include "Window/Qt/QtWindow.h"
#include "Window/Qt/glWidget.h"
#include "Planet.h"
#include "PlanetWidget.h"

class FrameBuffer;
class GUI;
class Camera;
class Light;

class PlanetsApp: public QApplication {

Q_OBJECT

public:
  vector<Planet*> planets;
  GLWidget *glWidget;
  Planet* focusedPlanet;
  GUI* gui;
  Material *HDR;
  bool usePostprocessing;
  bool useWireframe;
  Camera* camera;
  Light * light;
  FrameBuffer *fbo;
  QtWindow * window;
  QTimer *drawTimer;

  PlanetsApp(int &argc, char **argv);
  ~PlanetsApp();

  PlanetWidget * focusPlanet();
  void scene();
  void initCamAndLight();
  void initWidgets(QHBoxLayout * mainLayout);
  void initPostProcessing();
  void drawPlanets();
  void startPass();
  void endPass();

public slots:
  void draw();
  void setLazy(bool lazy);
  void setExposure(double exposure);
  void setWireframe(bool wire);
  void setPostprocessing(bool post);
};
#endif /* PLANETSAPP_H_ */
