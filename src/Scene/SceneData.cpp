/*
 * SceneData.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: bmonkey
 */

#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "Renderer/RenderEngine.h"
#include "Material/Materials.h"

SceneData::SceneData() {
  lights = QMap<string, Light*>();
  useMultiLights = false;
}

SceneData::~SceneData() {
  // TODO Auto-generated destructor stub
}
void SceneData::initLightBuffer(const string& shaderName, const string& bufferName) {
  ShaderProgram * shader = SceneData::Instance().getProgram(shaderName);
  initLightBuffer(shader, bufferName);
}

void SceneData::initLightBuffer(ShaderProgram * shader, const string& bufferName) {
  lightBuffer = new UniformBuffer();
  SceneData::Instance().lightBuffer->bind();

  GLuint uniBlockIndex = glGetUniformBlockIndex(shader->getHandle(), bufferName.c_str());
  glGetActiveUniformBlockiv(
    shader->getHandle(),
    uniBlockIndex,
    GL_UNIFORM_BLOCK_DATA_SIZE,
    &lightBufferSize
  );

  printf("Light Uniform Buffer Size %d\n", lightBufferSize);


  unsigned lightIndex = 0;
  foreach(Light* light, lights){

//    lightBufferData[lightIndex].position = getCurrentCamera()->getView() * light->position;
    lightBufferData[lightIndex].position = light->position;
    lightBufferData[lightIndex].diffuse = light->diffuse;
    lightBufferData[lightIndex].specular = light->specular;
    lightBufferData[lightIndex].direction = light->direction;

    printf("Found Light %s\n", lights.key(light).c_str());
    qDebug() << lightBufferData[lightIndex].diffuse;

    lightIndex++;
    glError;
  }
  lightBuffer->write(lightBufferData, lightBufferSize);

  shader->bindUniformBuffer(bufferName,0,lightBuffer->getHandle());
  useMultiLights = true;
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
  LogWarning << "Program not found" << name;
    return new ShaderProgram();
  }
}

Material* SceneData::getMaterial(const string & name) {
  Material * material = materials[name];
  if (material) {
    return material;
  } else {
    LogWarning << "Material not found"<< name;
    return new Minimal();
  }
}

void SceneData::setResolution(unsigned width, unsigned height) {
  this->height = height;
  this->width = width;
}

void SceneData::addLight(const string & name, Light* light) {
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

Light* SceneData::getLight(const string & name) {
  Light * light = lights[name];
  if (light) {
    return light;
  } else {
    LogWarning <<  "Light not found" << name;
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
  if(!shadowLight) {
    LogError << "No Shadow Light";
  }

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

void SceneData::updateLightBuffer() {
  if(!useMultiLights) return;
  unsigned lightIndex = 0;
  foreach(Light* light, lights) {
      lightBufferData[lightIndex].position = getCurrentCamera()->getView() * light->position;
      lightIndex++;
  }
  lightBuffer->write(lightBufferData, lightBufferSize);
}
