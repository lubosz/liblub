/*
 * PlanetsApp.h
 *
 *  Created on: Aug 27, 2011
 *      Author: bmonkey
 */

#ifndef PLANETSAPP_H_
#define PLANETSAPP_H_

//#include <QtGui>

#include "System/Demo.h"
#include "Procedural/Planets/Planet.h"

class FrameBuffer;
class FontOverlay;
class Camera;
class Light;

class PlanetsDemo: public Demo {

//Q_OBJECT

public:
  vector<Planet*> planets;
  Planet* focusedPlanet;
  ShaderProgram *HDR;
  bool usePostprocessing;
  Camera* camera;
  Light * light;
  FrameBuffer *fbo;
  Mesh * fullPlane;

  PlanetsDemo();
  ~PlanetsDemo();

  bool useWireframe;

  void init();
  void draw();
  void initCamAndLight();
  void initPostProcessing();
  void drawPlanets();
  void startPass();
  void endPass();

};
#endif /* PLANETSAPP_H_ */
