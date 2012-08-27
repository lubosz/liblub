/*
 * Copyright © 2010 Lubosz Sarnecki
 * SceneLoader.cpp
 *
 *  Created on: Dec 16, 2010
 */
#include <QStringList>
#include <QFile>
#include <vector>
#include <string>
#include "Scene/SceneLoader.h"
#include "Scene/Scene.h"
#include "Mesh/MeshLoader.h"
#include "Mesh/MengerSponge.h"
#include "Mesh/Geometry.h"
#include "System/Config.h"
#include "System/Logger.h"
#include "Material/Textures.h"
#include "Material/Shaders.h"
#include "Scene/SceneGraph.h"

SceneLoader::SceneLoader(const QString & fileName)
:
    fileName(fileName) {
}

SceneLoader::~SceneLoader() {
    // TODO(bmonkey) Auto-generated destructor stub
}

QVector3D SceneLoader::stringToVector3D(const QString& values) {
    vector<float> floats = splitValues<float> (values);
    return QVector3D(floats[0], floats[1], floats[2]);
}

void SceneLoader::appendProgram(const QDomElement & programNode) {
    string name, shaderUrl;
    vector<string> flags;
    QList<string> attributes;
    attributes.push_back("uv");
    attributes.push_back("normal");
    attributes.push_back("tangent");
    attributes.push_back("bitangent");

    if (programNode.hasAttribute("name"))
        name = programNode.attribute("name").toStdString();

    ShaderProgram * program = new ShaderProgram(name);

    QDomElement programInfo = programNode.firstChildElement();
    while (!programInfo.isNull()) {
        if (programInfo.tagName() == "Shader") {
            shaderUrl = programInfo.attribute("url").toStdString();
            if (programInfo.hasAttribute("type")) {
                QString shaderType = programInfo.attribute("type");
                if (shaderType == "VERTEX_SHADER")
                    program->attachShader(shaderUrl, GL_VERTEX_SHADER, false);
                else if (shaderType == "GEOMETRY_SHADER")
                    program->attachShader(shaderUrl, GL_GEOMETRY_SHADER, false);
                else if (shaderType == "FRAGMENT_SHADER")
                    program->attachShader(shaderUrl, GL_FRAGMENT_SHADER, false);
                else if (shaderType == "CONTROL_SHADER")
                    program->attachShader(shaderUrl, GL_TESS_CONTROL_SHADER, false);
                else if (shaderType == "EVALUATION_SHADER")
                    program->attachShader(shaderUrl, GL_TESS_EVALUATION_SHADER, false);
            } else {
                if (programInfo.hasAttribute("flags")) {
                    flags
                            = splitValues<string> (programInfo.attribute(
                                    "flags"));
                    program->attachVertFrag(shaderUrl, flags);
                } else {
                    program->attachVertFrag(shaderUrl, false);
                }
            }
        } else if (programInfo.tagName() == "Template") {
          shaderUrl = programInfo.attribute("url").toStdString();
          program->attachVertFrag(shaderUrl, true);
        } else if (programInfo.tagName() == "Uniform") {
            program->uniforms.push_back(Uniform<float> (programInfo.attribute(
                    "name").toStdString(), splitValues<float> (
                    programInfo.attribute("value"))));
        } else if (programInfo.tagName() == "Uniformi") {
            program->uniformsi.push_back(Uniform<int> (programInfo.attribute(
                    "name").toStdString(), splitValues<int> (
                    programInfo.attribute("value"))));
        } else if (programInfo.tagName() == "Layer") {
            if (programInfo.hasAttribute("texture")) {
                Texture * texture = Scene::Instance().textures.value(
                        programInfo.attribute("texture").toStdString());
                if (Scene::Instance().textures.count(programInfo.attribute("texture").toStdString()) == 0)
                          LogError << "Texture "
                                    << programInfo.attribute("texture").toStdString()
                                    << " not found.";
                // TODO(bmonkey): uniform name reset
                LogDebug << programInfo.attribute("sampler").toStdString();
                texture->name = programInfo.attribute("sampler").toStdString();
                program->addTexture(texture);
            }
        }
        programInfo = programInfo.nextSiblingElement();
    }

    program->init(attributes);
    program->samplerUniforms();
    Scene::Instance().shaders.insert(name, program);
}

void SceneLoader::appendTexture(const QDomElement & textureNode) {
    string name = textureNode.attribute("name").toStdString();
    LogDebug << "Texture name:" << name;
    Texture * texture;
    if (textureNode.tagName() == "Texture") {
        texture = new TextureFile(
                textureNode.attribute("url").toStdString(), name);
    } else if (textureNode.tagName() == "TextureCube") {
        texture = new CubeTextureFile(textureNode.attribute(
                "url").toStdString(), name);
    } else texture = NULL;
    Scene::Instance().textures.insert(name, texture);
}

void SceneLoader::appendMesh(const QDomElement & meshNode) {
    string name = meshNode.attribute("name").toStdString();
    LogDebug << "Mesh name:" << name;
    Mesh *mesh = NULL;

    if (meshNode.tagName() == "File") {
        string meshUrl = meshNode.attribute("url").toStdString();
        QList<string> attributes;
        attributes.push_back("normal");
        attributes.push_back("tangent");
        attributes.push_back("bitangent");
        attributes.push_back("uv");
            mesh = MeshLoader::load(attributes, meshUrl);
    } else if (meshNode.tagName() == "Procedural") {
        if (meshNode.attribute("type") == "Sponge") {
          QList<string> attributes;
          attributes.push_back("normal");
          MengerSponge * sponge = new MengerSponge(attributes, meshNode.attribute(
                    "recursion").toInt());
          mesh = sponge->getMesh();
        } else if (meshNode.attribute("type") == "Stars") {
          QList<string> attributes;
          attributes.push_back("color");
          float resolution = meshNode.attribute("resolution").toFloat();
          QVector3D resolutionVec =
                  QVector3D(resolution, resolution, resolution);
            mesh = Geometry::stars(
                attributes,
                    resolutionVec,
                    meshNode.attribute("density").toFloat(),
                    meshNode.attribute("randomness").toFloat(),
                    meshNode.attribute("colorIntensity").toFloat());
        } else if (meshNode.attribute("type") == "Tetrahedron") {
          QList<string> attributes;
          attributes.push_back("color");
            mesh = Geometry::tetrahedron(attributes);
        } else if (meshNode.attribute("type") == "Cube") {
          QList<string> attributes;
          attributes.push_back("color");
            mesh = Geometry::cube(attributes);
        } else if (meshNode.attribute("type") == "Plane") {
          QList<string> attributes;
          attributes.push_back("color");
            mesh = Geometry::plane(
                attributes,
                QRectF(
                    meshNode.attribute("left").toFloat(),
                    meshNode.attribute("top").toFloat(),
                    meshNode.attribute("width").toFloat(),
                    meshNode.attribute("height").toFloat()));
        } else if (meshNode.attribute("type") == "Sphere") {
          QList<string> attributes;
          attributes.push_back("color");
            mesh = Geometry::sphere(
                attributes,
                    meshNode.attribute("radius").toFloat(),
                    meshNode.attribute("slices").toInt(),
                    meshNode.attribute("stacks").toInt());
        } else if (meshNode.attribute("type") == "Icosahedron") {
          QList<string> attributes;
          attributes.push_back("color");
            mesh = Geometry::icosahedron(attributes);
        } else if (meshNode.attribute("type") == "Spiral") {
          QList<string> attributes;
          attributes.push_back("color");
            mesh = Geometry::spiral(
                attributes,
                    meshNode.attribute("resolution").toFloat());
        } else {
          QList<string> attributes;
          attributes.push_back("color");
            mesh = Geometry::cube(attributes);
          LogError << "Unknown Mesh Type" << meshNode.attribute("type").toStdString();
        }

    }
    if (meshNode.hasAttribute("drawType")) {
      GLint drawType = GL_TRIANGLES;
      string drawTypeString = meshNode.attribute("drawType").toStdString();

      if (drawTypeString == "PATCHES")
        drawType = GL_PATCHES;
      else if (drawTypeString == "POINTS")
        drawType = GL_POINTS;
      else if (drawTypeString == "TRIANGLE_STRIP")
        drawType = GL_TRIANGLE_STRIP;
      else if (drawTypeString == "TRIANGLES")
        drawType = GL_TRIANGLES;
      else if (drawTypeString == "LINES")
        drawType = GL_LINES;

      mesh->setDrawType(drawType);
    }

    Scene::Instance().meshes.insert(name, mesh);
}

void SceneLoader::appendMaterial(const QDomElement & materialNode) {

}

void SceneLoader::appendObject(const QDomElement & objectNode) {
    QVector3D position, direction, rotation;
    string name;
    float scale = 1;
    ShaderProgram * material = NULL;
    Mesh * mesh = NULL;

    if (objectNode.hasAttribute("position"))
        position = stringToVector3D(objectNode.attribute("position"));
    if (objectNode.hasAttribute("direction"))
        direction = stringToVector3D(objectNode.attribute("direction"));
    if (objectNode.hasAttribute("rotation"))
      rotation = stringToVector3D(objectNode.attribute("rotation"));
    else
      rotation = QVector3D();
    if (objectNode.hasAttribute("name"))
        name = objectNode.attribute("name").toStdString();
    if (objectNode.hasAttribute("scale"))
        scale = objectNode.attribute("scale").toFloat();
    if (objectNode.hasAttribute("material")) {
        string materialName = objectNode.attribute("material").toStdString();
        if (Scene::Instance().shaders.count(materialName) > 0)
            material = Scene::Instance().shaders.value(materialName);
        else
            LogError << "Shader Not Found" << materialName;
    }
    if (objectNode.tagName() == "Light") {
      string lightName;
      Light * light = new Light(position, direction);

      if (objectNode.hasAttribute("name"))
        lightName = objectNode.attribute("name").toStdString();

      if (objectNode.hasAttribute("diffuse"))
        light->diffuse = stringToVector3D(objectNode.attribute("diffuse"));

      if (objectNode.hasAttribute("specular"))
        light->specular = stringToVector3D(objectNode.attribute("specular"));

      Scene::Instance().addLight(lightName, light);
      if (objectNode.hasAttribute("mesh")) {
        string meshName = objectNode.attribute("mesh").toStdString();
        if (Scene::Instance().meshes.count(meshName) > 0)
          mesh = Scene::Instance().meshes.value(meshName);
        else
          LogError << "Mesh Not Found" << meshName;

        ShaderProgram * lightMat;
        if (objectNode.hasAttribute("material")) {
          lightMat = material;
        } else {
          lightMat = new SimpleProgram("Color/white", QList<string>());
        }
        Node * lightNode = new Node("Light", position, 1, mesh, lightMat);
        lightNode->setCastShadows(false);
        SceneGraph::Instance().addNode(lightNode);
      }
    } else if (objectNode.tagName() == "Object") {
        string meshName = objectNode.attribute("mesh").toStdString();
        if (Scene::Instance().meshes.count(meshName) > 0)
            mesh = Scene::Instance().meshes.value(meshName);
        else
            LogError << "Mesh Not Found"<< meshName;

        Node * node = new Node(name, position, scale, mesh, material);
        if (objectNode.attribute("receive_shadows").contains("true",
                Qt::CaseInsensitive))
            node->setReceiveShadows(true);
        else if (objectNode.attribute("receive_shadows").contains("false",
                Qt::CaseInsensitive))
            node->setReceiveShadows(false);

        if (objectNode.attribute("cast_shadows").contains("true",
                Qt::CaseInsensitive))
            node->setCastShadows(true);
        else if (objectNode.attribute("cast_shadows").contains("false",
                Qt::CaseInsensitive))
            node->setCastShadows(false);

        if (objectNode.attribute("transparent").contains("true",
                Qt::CaseInsensitive))
            node->setTransparent(true);

        node->setRotation(rotation);

        SceneGraph::Instance().addNode(node);
    } else if (objectNode.tagName() == "ObjectPlane") {
        string meshName = objectNode.attribute("mesh").toStdString();
        if (Scene::Instance().meshes.count(meshName) > 0)
            mesh = Scene::Instance().meshes.value(meshName);
        else
            LogError << "Mesh Not Found"<< meshName;

        vector<string> materialNames =
            splitValues<string> (objectNode.attribute("materials"));

        vector<ShaderProgram*> planeMaterials;

        foreach(string materialName, materialNames) {
          if (Scene::Instance().shaders.count(materialName) > 0)
            planeMaterials.push_back(Scene::Instance().shaders.value(materialName));
          else
            LogError << "Shader Not Found"<< materialName;
        }

        SceneGraph::Instance().meshPlane(
                mesh,
                objectNode.attribute("size").toFloat(),
                objectNode.attribute("step").toFloat(),
                planeMaterials);
    }
}

void SceneLoader::load() {
    load(fileName);
}

void SceneLoader::load(const QString & fileName) {
  QString filepath = QString(Config::Instance().value<string>("sceneDir").c_str())
                + fileName;
    QFile file(filepath);

    if (!file.exists())
      LogFatal << filepath.toStdString() << "does not exist.";

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument domDocument;
    domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn);

    if(errorStr != "")
      LogFatal << errorStr.toStdString();

    sceneXML = domDocument.documentElement();

    QDomElement document = sceneXML.firstChildElement();
    while (!document.isNull()) {
        if (document.tagName() == "Programs") {
            QDomElement programs = document.firstChildElement();
            while (!programs.isNull()) {
                appendProgram(programs);
                programs = programs.nextSiblingElement();
            }
        } else if (document.tagName() == "Textures") {
            QDomElement textures = document.firstChildElement();
            while (!textures.isNull()) {
                appendTexture(textures);
                textures = textures.nextSiblingElement();
            }
        } else if (document.tagName() == "Meshes") {
            QDomElement meshes = document.firstChildElement();
            while (!meshes.isNull()) {
                appendMesh(meshes);
                meshes = meshes.nextSiblingElement();
            }
        } else if (document.tagName() == "Scene") {

          if (document.hasAttribute("name")){
              Scene::Instance().name = document.attribute("name").toStdString();
              LogInfo << "Loading Scene" << Scene::Instance().name;
          }
          if (document.hasAttribute("backgroundColor")){
              Scene::Instance().setBackgroundColor(stringToVector3D(document.attribute("backgroundColor")));
          } else {
            vector<float> backgroundColor =
                        Config::Instance().values<float> ("backgroundColor");
            Scene::Instance().setBackgroundColor(
                QVector3D(backgroundColor[0],backgroundColor[1],backgroundColor[2]));
          }
            QDomElement scene = document.firstChildElement();
            while (!scene.isNull()) {
                appendObject(scene);
                scene = scene.nextSiblingElement();
            }
        } else if (document.tagName() == "include") {
            load(document.attribute("url"));
        }
        document = document.nextSiblingElement();
    }
}
