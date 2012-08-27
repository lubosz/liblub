/*
 * PlanetsApp.h
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#ifndef PLANETSAPP_H_
#define PLANETSAPP_H_

#include <QtGui>

#include "Window/Qt/QtWindow.h"
#include "Window/Qt/glWidget.h"
#include "Window/Qt/QtApplication.h"
#include "Planet.h"
#include "PlanetWidget.h"


class FrameBuffer;
class FontOverlay;
class Camera;
class Light;

class PlanetsApp: public QtApplication {

Q_OBJECT

public:
  vector<Planet*> planets;
  Planet* focusedPlanet;
  ShaderProgram *HDR;
  bool usePostprocessing;
  Camera* camera;
  Light * light;
  FrameBuffer *fbo;
  Mesh * fullPlane;

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
  void renderFrame();

public slots:
  void setExposure(double exposure);
  void setPostprocessing(bool post);
};
#endif /* PLANETSAPP_H_ */
