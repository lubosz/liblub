/*
 * SceneData.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: bmonkey
 */

#include "Scene/Scene.h"
#include "System/Logger.h"
#include "Renderer/OpenGL.h"
#include "Shader/Shaders.h"
#include "Texture/Textures.h"
#include "Procedural/Geometry.h"
#include "Scene/SceneGraph.h"
#include "System/Config.h"

Scene::Scene() {
  lights = QMap<string, Light*>();
  currentCamera = nullptr;
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
  bool isInCameras = false;

  foreach(Camera* cam, cameras) {
    if (cam == camera)
      isInCameras = true;
  }

  if (!isInCameras) {
      addCamera(camera);
  }
}

void Scene::addCamera(const QVector3D & pos, const QVector3D & dir) {
    addCamera(new Camera(pos, dir));
}

void Scene::addCamera(Camera * camera) {
    this->currentCamera = camera;
    QString camname = "camera" + QString::number(cameras.size() + 1);
    cameras.insert(camname.toStdString(), camera);
    LogDebug << "Adding " << camname.toStdString();
}

void Scene::initSkyMaterial(const string& textureName) {
    new CubeTextureFile(textureName, "sky");
    Material *skyMat = new Material("sky");
    QList<string> attributes = QList<string> () << "uv" << "normal" << "tangent" << "bitangent";
    Mesh * sphere = Geometry::sphere(attributes, 500, 20, 20);
    Node * skyNode = new Node("skynode", QVector3D(0,0,0),1,  sphere, skyMat);
    SceneGraph::Instance().addNode(skyNode);
}

void Scene::initSkyShader(const string& textureName) {
    Texture * skyTex = new CubeTextureFile(textureName, "EnvMap");
    QList<string> attributes = QList<string> () << "uv";
    ShaderProgram * skyShader = new VertFragProgram("Texture/cubemapSky", attributes);
    skyShader->addTexture(skyTex);

    Mesh * sphere = Geometry::sphere(attributes, 1.0, 20, 20);
    Node * skyNode = new Node("skynode", Scene::Instance().getCurrentCamera()->getPosition(),1.0,  sphere, skyShader);
    skyNode->setSize(Scene::Instance().getCurrentCamera()->farClip * 0.4);

    SceneGraph::Instance().addNode(skyNode);
}

void Scene::initSkyShaderSphere(const string& textureName) {
    Texture * skyTex = new TextureFile(textureName, "envMap");
    QList<string> attributes = QList<string> () << "uv";
    ShaderProgram * skyShader = new VertFragProgram("Texture/sphereMapSky", attributes);
    skyShader->addTexture(skyTex);

    Mesh * sphere = Geometry::sphere(attributes, 1.0, 20, 20);
    Node * skyNode = new Node("skynode", Scene::Instance().getCurrentCamera()->getPosition(),1.0,  sphere, skyShader);
    skyNode->setSize(Scene::Instance().getCurrentCamera()->farClip * 0.4);

    SceneGraph::Instance().addNode(skyNode);
}

ShaderProgram* Scene::getShader(const string & name) {
  ShaderProgram * shader = shaders[name];
  if (shader) {
    return shader;
  } else {
  LogWarning << "Program not found" << name;
    return new ShaderProgram("Empty");
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
}

Light* Scene::getLight(const string & name) {
  Light * light = lights[name];
  if (light) {
    return light;
  } else {
    LogWarning <<  "Light not found" << name;
    return getDefaultLight();
  }
}

Light* Scene::getDefaultLight() {
    if (lights.size() > 0) {
        return lights.values().at(0);
    } else {
        LogError << "No Light Found";
        Light * light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
        lights.insert("nonamelight", light);
        return light;
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

void Scene::initLightBuffer(ShaderProgram * shader, const string& bufferName) {
    vector<int> GLcontext = Config::Instance().values<int>("GLcontext");

    if ((GLcontext[0] == 3 && GLcontext[1] >= 1) || GLcontext[0] > 3) {

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
    } else {
        LogWarning << "Light Buffer not initialized. GL Version" << GLcontext[0] << "." << GLcontext[1];
    }
}
