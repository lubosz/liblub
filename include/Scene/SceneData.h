/*
 * SceneData.h
 *
 *  Created on: Mar 4, 2011
 *      Author: bmonkey
 */

#pragma once

#include "common/Singleton.h"

#include <string>
#include <QMap>
#include "Material/ShaderProgram.h"
#include "Material/Texture.h"
#include "Material/Material.h"
#include "Mesh/Mesh.h"

using std::string;

class SceneData : public Singleton<SceneData> {
public:
  string name;

  QMap<string, ShaderProgram*> shaderPrograms;
  QMap<string, Texture*> textures;
  QMap<string, Material*> materials;
  QMap<string, Mesh*> meshes;

  SceneData();
  virtual ~SceneData();

  void addProgram(string & name, ShaderProgram * program);
  ShaderProgram* getProgram(const string & name);
};

