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
#include "Scene/SceneData.h"
#include "Mesh/MeshLoader.h"
#include "Mesh/MengerSponge.h"
#include "Mesh/Geometry.h"
#include "System/Config.h"
#include "System/Logger.h"

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
    ShaderProgram * program = new ShaderProgram();

    if (programNode.hasAttribute("name"))
        name = programNode.attribute("name").toStdString();

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
        } else if (programInfo.tagName() == "Uniform") {
            program->uniforms.push_back(Uniform<float> (programInfo.attribute(
                    "name").toStdString(), splitValues<float> (
                    programInfo.attribute("value"))));
        } else if (programInfo.tagName() == "Uniformi") {
            program->uniformsi.push_back(Uniform<int> (programInfo.attribute(
                    "name").toStdString(), splitValues<int> (
                    programInfo.attribute("value"))));
        }
        programInfo = programInfo.nextSiblingElement();
    }

    QList<string> attributes;
    attributes.push_back("uv");
    attributes.push_back("normal");
    attributes.push_back("tangent");

    program->init(attributes);
    program->name = name;
    SceneData::Instance().shaderPrograms.insert(name, program);
}

void SceneLoader::appendMaterial(const QDomElement & materialNode) {
    string name, program;
    vector<string> layerStrings;
    Material * material = new EmptyMat();

    if (materialNode.hasAttribute("name"))
        name = materialNode.attribute("name").toStdString();
    if (materialNode.hasAttribute("program")) {
        program = materialNode.attribute("program").toStdString();
        if (SceneData::Instance().shaderPrograms.count(program) > 0)
            material->shaderProgram = SceneData::Instance().shaderPrograms.value(program);
        else
            LogError << "Program Not Found"<< program;
    } else {
      LogError << "NO SHADER PROGRAM";
    }

    QDomElement layers = materialNode.firstChildElement();
    while (!layers.isNull()) {
        if (layers.hasAttribute("texture")) {
            Texture * texture = SceneData::Instance().textures.value(
                    layers.attribute("texture").toStdString());
            if (SceneData::Instance().textures.count(layers.attribute("texture").toStdString()) == 0)
              LogError << "Texture "
                        << layers.attribute("texture").toStdString()
                        << " not found.";
            // TODO(bmonkey): uniform name reset
            LogDebug << layers.attribute("sampler").toStdString();
            texture->name = layers.attribute("sampler").toStdString();
            material->addTexture(texture);
        }
        layers = layers.nextSiblingElement();
    }
#ifdef USE_FBO
    // TODO(bmonkey): init shadow map texutre only when needed
//    material->addTexture(
//            RenderEngine::Instance().shadowSequence->
//            renderPasses[0]->targetTexture);
#endif
    material->shaderProgram->use();
    material->samplerUniforms();
    SceneData::Instance().materials.insert(name, material);
}

void SceneLoader::appendTexture(const QDomElement & textureNode) {
    string name = textureNode.attribute("name").toStdString();
    LogDebug << "Texture name:" << name;
    Texture * texture;
    if (textureNode.tagName() == "Texture") {
        texture = TextureFactory::Instance().load(
                textureNode.attribute("url").toStdString(), name);
    } else if (textureNode.tagName() == "TextureCube") {
        texture = TextureFactory::Instance().loadCubeMap(textureNode.attribute(
                "url").toStdString(), name);
    }
    SceneData::Instance().textures.insert(name, texture);
}

void SceneLoader::appendMesh(const QDomElement & meshNode) {
    string name = meshNode.attribute("name").toStdString();
    LogDebug << "Mesh name:" << name;
    Mesh *mesh;

    if (meshNode.tagName() == "File") {
        string meshUrl = meshNode.attribute("url").toStdString();
        QList<string> attributes;
        attributes.push_back("normal");
        attributes.push_back("tangent");
        attributes.push_back("bitangent");
        attributes.push_back("uv");

        if (meshNode.hasAttribute("drawType")) {
            GLint drawType = GL_TRIANGLES;
            string drawTypeString =
                    meshNode.attribute("drawType").toStdString();

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

            mesh = MeshLoader::load(attributes, meshUrl);
            mesh->setDrawType(drawType);
        } else {
            mesh = MeshLoader::load(attributes, meshUrl);
        }
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
          vector<float> resolutionvec = { resolution, resolution, resolution };
            mesh = Geometry::stars(
                attributes,
                    resolutionvec,
                    meshNode.attribute("density").toFloat(),
                    meshNode.attribute("randomness").toFloat(),
                    meshNode.attribute("colorIntensity").toFloat());
        } else if (meshNode.attribute("type") == "Spiral") {
          QList<string> attributes;
          attributes.push_back("color");
            mesh = Geometry::spiral(
                attributes,
                    meshNode.attribute("resolution").toFloat());
        }
    }

    SceneData::Instance().meshes.insert(name, mesh);
}

void SceneLoader::appendObject(const QDomElement & objectNode) {
    QVector3D position, direction, rotation;
    string name;
    float scale = 1;
    Material * material = NULL;
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
        if (SceneData::Instance().materials.count(materialName) > 0)
            material = SceneData::Instance().materials.value(materialName);
        else
            LogError << "Material Not Found" << materialName;
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

      SceneData::Instance().addLight(lightName, light);
      if (objectNode.hasAttribute("mesh")) {
        string meshName = objectNode.attribute("mesh").toStdString();
        if (SceneData::Instance().meshes.count(meshName) > 0)
          mesh = SceneData::Instance().meshes.value(meshName);
        else
          LogError << "Mesh Not Found" << meshName;

        Material * lightMat;
        if (objectNode.hasAttribute("material")) {
          lightMat = material;
        } else {
          lightMat = new Simple("Color/white", QList<string>());
        }
        Node * lightNode = new Node("Light", position, 1, mesh, lightMat);
        lightNode->setCastShadows(false);
        SceneGraph::Instance().addNode(lightNode);
      }
    } else if (objectNode.tagName() == "Object") {
        string meshName = objectNode.attribute("mesh").toStdString();
        if (SceneData::Instance().meshes.count(meshName) > 0)
            mesh = SceneData::Instance().meshes.value(meshName);
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
            node->transparent = true;

        node->setRotation(rotation);

        SceneGraph::Instance().addNode(node);
    } else if (objectNode.tagName() == "ObjectPlane") {
        string meshName = objectNode.attribute("mesh").toStdString();
        if (SceneData::Instance().meshes.count(meshName) > 0)
            mesh = SceneData::Instance().meshes.value(meshName);
        else
            LogError << "Mesh Not Found"<< meshName;

        vector<string> materialNames =
            splitValues<string> (objectNode.attribute("materials"));

        vector<Material*> planeMaterials;

        foreach(string materialName, materialNames) {
          if (SceneData::Instance().materials.count(materialName) > 0)
            planeMaterials.push_back(SceneData::Instance().materials.value(materialName));
          else
            LogError << "Material Not Found"<< materialName;
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
    QFile file(QString(Config::Instance().value<string> ("sceneDir").c_str())
            + fileName);

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument domDocument;
    domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn);

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
        } else if (document.tagName() == "Materials") {
            QDomElement materials = document.firstChildElement();
            while (!materials.isNull()) {
                appendMaterial(materials);
                materials = materials.nextSiblingElement();
            }
        } else if (document.tagName() == "Scene") {

          if (document.hasAttribute("name")){
              SceneData::Instance().name = document.attribute("name").toStdString();
              LogInfo << "Loading Scene" << SceneData::Instance().name;
          }
          if (document.hasAttribute("backgroundColor")){
              SceneData::Instance().setBackgroundColor(stringToVector3D(document.attribute("backgroundColor")));
          } else {
            vector<float> backgroundColor =
                        Config::Instance().values<float> ("backgroundColor");
            SceneData::Instance().setBackgroundColor(
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
