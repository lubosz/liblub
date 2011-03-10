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
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Mesh/Mesh.h"

using std::string;

class SceneData : public Singleton<SceneData> {
public:
  string name;
  QVector3D backgroundColor;

  QMap<string, ShaderProgram*> shaderPrograms;
  QMap<string, Texture*> textures;
  QMap<string, Material*> materials;
  QMap<string, Mesh*> meshes;
  QMap<string, Camera*> cameras;

  SceneData();
  virtual ~SceneData();
  void setBackgroundColor(const QVector3D backgroundColor);

  void addProgram(string & name, ShaderProgram * program);
  ShaderProgram* getProgram(const string & name);

  void addLight(string & name, Light * light);
  Light* getLight(const string & name);

   Camera * getCurrentCamera();
   Light * getShadowLight();
   Light * getMoveLight();
private:
  Camera *currentCamera;
  QMap<string, Light*> lights;
  Light *shadowLight, *moveLight;
};

