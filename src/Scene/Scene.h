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
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Mesh/Mesh.h"
#include "Material/UniformBuffer.h"

using std::string;

class Scene : public Singleton<Scene> {
public:
  string name;
  QVector3D backgroundColor;
  unsigned width, height;

  QMap<string, ShaderProgram*> shaders;
  QMap<string, Texture*> textures;
  QMap<string, Mesh*> meshes;
  QMap<string, Camera*> cameras;
  QMap<string, Light*> lights;

  Scene();
  virtual ~Scene();
  void setBackgroundColor(const QVector3D backgroundColor);

  void addShader(string & name, ShaderProgram * program);
  ShaderProgram* getShader(const string & name);

  void addLight(const string & name, Light * light);
  Light* getLight(const string & name);
  Texture* getTexture(const string & name);
  Texture* getTexture(const string & name, const string & as);

  void setResolution(unsigned width, unsigned height);

  QSize getResolution();

  Camera * getCurrentCamera();
  Light * getShadowLight();
  void setShadowLight(Light * light);
  Light * getMoveLight();

  void setCurrentCamera(Camera * camera);
  void addCamera(Camera * camera);

  struct LightSource {
    QVector4D position;
    QVector4D diffuse;
    QVector4D specular;
    QVector4D direction;
  };

  LightSource lightBufferData[10];
  UniformBuffer * lightBuffer;
  GLint lightBufferSize;
  void initLightBuffer(ShaderProgram * shader, const string& bufferName);

private:
  Camera *currentCamera;
  Light *shadowLight, *moveLight;
};

