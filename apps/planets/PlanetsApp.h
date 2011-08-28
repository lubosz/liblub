/*
 * PlanetsApp.h
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#ifndef PLANETSAPP_H_
#define PLANETSAPP_H_

#include <string>
#include <QApplication>
#include <QPainter>
#include <QtGui>
#include <QtCore>
#include "System/TemplateEngine.h"
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Material/ProcTextures.h"
#include "Material/Textures.h"
#include "Material/ShaderProgram.h"
#include "Planet.h"
#include "PlanetWindow.h"
#include "PlanetWidget.h"
#include "FloatEditorWidget.h"
#include "glWidget.h"

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
  PlanetWindow * window;

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
