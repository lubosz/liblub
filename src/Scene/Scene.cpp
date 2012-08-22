/*
 * SceneData.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: bmonkey
 */

#include "Scene/Scene.h"
#include "System/Logger.h"
#include "Renderer/OpenGL.h"
#include "Material/Shaders.h"

Scene::Scene() {
  lights = QMap<string, Light*>();
  shadowLight = nullptr;
  currentCamera = nullptr;
  moveLight = nullptr;
}

Scene::~Scene() {
  foreach(Camera* camera, cameras)
    delete camera;
  foreach(Light* light, lights)
    delete light;
}

void Scene::addShader(string & name, ShaderProgram* shader) {
  if (name == "") {
    QString idString = QString::number(shaders.size() + 1);
    name = "Program" + idString.toStdString();
  }
  shaders.insert(name, shader);
}

void Scene::setCurrentCamera(Camera * camera) {
  this->currentCamera = camera;
  addCamera(camera);
}

void Scene::addCamera(Camera * camera) {
  bool isInCameras = false;

  foreach(Camera* cam, cameras) {
    if (cam == camera)
      isInCameras = true;
  }

  if (!isInCameras) {
    QString camname = "camera" + QString::number(cameras.size() + 1);
    cameras.insert(camname.toStdString(), camera);
    LogDebug << "Adding " << camname.toStdString();

  }
}

ShaderProgram* Scene::getShader(const string & name) {
  ShaderProgram * shader = shaders[name];
  if (shader) {
    return shader;
  } else {
  LogWarning << "Program not found" << name;
    return new EmptyProgram();
  }
}

void Scene::setResolution(unsigned width, unsigned height) {
  this->height = height;
  this->width = width;
}

QSize Scene::getResolution() {
  if(width == 0|| height == 0)
    LogFatal << "No Window Resulution!";
  return QSize(width, height);
}

void Scene::addLight(const string & name, Light* light) {
  light->setName(name);
  string lightName = name;
  if (name == "" || lights.count(name) > 0) {
    QString idString = QString::number(lights.size() + 1);
    lightName = "Light" + idString.toStdString();
  }
  lights.insert(lightName, light);

  if(!shadowLight)
    shadowLight = light;
  if(!moveLight)
    moveLight = light;

}

Light* Scene::getLight(const string & name) {
  Light * light = lights[name];
  if (light) {
    return light;
  } else {
    LogWarning <<  "Light not found" << name;
    return new Light();
  }
}


Texture* Scene::getTexture(const string & name) {
  Texture * texture = textures[name];
  if (texture) {
    return texture;
  } else {
    LogWarning <<  "Texture not found" << name;
    return new Texture("Dummy Texture");
  }
}

Texture* Scene::getTexture(const string & name, const string & as){
  Texture * texture = getTexture(name);
  texture->name = as;
  return texture;
}

void Scene::setBackgroundColor(const QVector3D backgroundColor) {
  this->backgroundColor = backgroundColor;
  OpenGL::Instance().setClearColor(backgroundColor);
}

Camera * Scene::getCurrentCamera() {
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

Light * Scene::getShadowLight() {
  if(!shadowLight) {
    LogWarning << "Adding default shadow light";
    Light * light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    addLight("ShadowLight", light);
  }
  return shadowLight;
}

void Scene::setShadowLight(Light * light) {
  shadowLight = light;
}

Light * Scene::getMoveLight() {
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

void Scene::initLightBuffer(ShaderProgram * shader, const string& bufferName) {
  lightBuffer = new UniformBuffer();
  lightBuffer->bind();

  #ifdef USE_OPENGL3
  GLuint uniBlockIndex = glGetUniformBlockIndex(shader->getHandle(), bufferName.c_str());
  glGetActiveUniformBlockiv(
    shader->getHandle(),
    uniBlockIndex,
    GL_UNIFORM_BLOCK_DATA_SIZE,
    &lightBufferSize
  );
#endif

  LogDebug << "Light Uniform Buffer Size" << lightBufferSize;

  unsigned lightIndex = 0;
  foreach(Light* light, Scene::Instance().lights){

    lightBufferData[lightIndex].position = light->position;
    lightBufferData[lightIndex].diffuse = light->diffuse;
    lightBufferData[lightIndex].specular = light->specular;
    lightBufferData[lightIndex].direction = light->direction();

    LogDebug << "Found Light" << Scene::Instance().lights.key(light);
//    qDebug() << lightBufferData[lightIndex].diffuse;

    lightIndex++;
    glError;
  }
  lightBuffer->write(lightBufferData, lightBufferSize);

//  shader->uniformBuffers.push_back(lightBuffer);
  #ifdef USE_OPENGL3
  shader->bindUniformBuffer(bufferName,0,lightBuffer->getHandle());
    #endif
}
