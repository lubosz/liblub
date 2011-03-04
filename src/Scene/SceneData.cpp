/*
 * SceneData.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: bmonkey
 */

#include "Scene/SceneData.h"
#include "System/Logger.h"

SceneData::SceneData() {
  // TODO Auto-generated constructor stub

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
