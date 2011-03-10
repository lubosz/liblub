/*
 * SceneData.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: bmonkey
 */

#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "Renderer/RenderEngine.h"

SceneData::SceneData() {
  // TODO Auto-generated constructor stub
  lights = QMap<string, Light*>();

}

SceneData::~SceneData() {
  // TODO Auto-generated destructor stub
}

void SceneData::addProgram(string & name, ShaderProgram* program) {
  if (name == "") {
    QString idString = QString::number(shaderPrograms.size() + 1);
    name = "Program" + idString.toStdString();
  }
  shaderPrograms.insert(name, program);
}

ShaderProgram* SceneData::getProgram(const string & name) {
  ShaderProgram * program = shaderPrograms[name];
  if (program) {
    return program;
  } else {
  Logger::Instance().log("WARNING", "Program not found", name);
    return new ShaderProgram();
  }
}

void SceneData::addLight(string & name, Light* light) {
  if (name == "" || lights.count(name) > 0) {
    QString idString = QString::number(lights.size() + 1);
    name = "Light" + idString.toStdString();
  }
  lights.insert(name, light);

  if(!shadowLight)
    shadowLight = light;
  if(!moveLight)
    moveLight = light;

}

Light* SceneData::getLight(const string & name) {
  Light * light = lights[name];
  if (light) {
    return light;
  } else {
  Logger::Instance().log("WARNING", "Light not found", name);
    return new Light();
  }
}

void SceneData::setBackgroundColor(const QVector3D backgroundColor) {
  this->backgroundColor = backgroundColor;
  RenderEngine::Instance().setClearColor(backgroundColor);
}

Camera * SceneData::getCurrentCamera() {
  if(!currentCamera) {
    if(cameras.size() > 0) {
      currentCamera = cameras[0];
    } else {
      Camera * camera = new Camera();
      currentCamera = camera;
      cameras.insert("camera",camera);
    }
  }

  return currentCamera;
}

Light * SceneData::getShadowLight() {
//  if(!shadowLight) {
//    if(lights.size() > 0) {
//      shadowLight = lights[0];
//    } else {
//      Light * light = new Light(QVector3D(), QVector3D(0,-1,0));
//      shadowLight = light;
//      lights.insert("light",light);
//    }
//  }

  return shadowLight;
}

Light * SceneData::getMoveLight() {
//  if(!moveLight) {
//    if(lights.size() > 0) {
//      Logger::Instance().message << lights.size();
//      Logger::Instance().log("DEBUG", "Light Size");
//      foreach(Light* light, lights) {
//        Logger::Instance().log("DEBUG", "Light Name", lights.key(light));
//        moveLight = light;
//      }
////      moveLight = lights[0];
//    } else {
//      Light * light = new Light(QVector3D(), QVector3D(0,-1,0));
//      moveLight = light;
//      lights.insert("light",light);
//    }
//  }

  return moveLight;
}
