/*
 * Copyright © 2010 Lubosz Sarnecki
 */
#include <vector>
#include <string>
#include "Scene/SceneGraph.h"
#include "Scene/Camera.h"
#include "Renderer/RenderEngine.h"
#include "Material/Materials.h"
#include "Mesh/MeshLoader.h"
#include "System/Logger.h"
#include "Scene/SceneData.h"
#include "System/GUI.h"

SceneGraph::SceneGraph() {
    bias = QMatrix4x4();
    bias.translate(.5, .5, .5);
    bias.scale(.5, .5, .5);
}

//void SceneGraph::animate(float frameCount) {
//    /*
//     rotate(modelmatrix, (GLfloat) frameCount * -1.0, X_AXIS);
//     rotate(modelmatrix, (GLfloat) frameCount * 1.0, Y_AXIS);
//     rotate(modelmatrix, (GLfloat) frameCount * 0.5, Z_AXIS);
//     */
//}

bool SceneGraph::hasNode(const string & name) {
  if (sceneNodes.count(name) > 0) {
    return true;
  } else {
    return false;
  }
}

Node * SceneGraph::getNode(const string & name){
  Node * node = sceneNodes.value(name);
  if (node) {
    return node;
  } else {
  LogWarning << "Node not found"<< name;
    return new Node();
  }
}

void SceneGraph::addNode(Node * node) {
  if (node->getName() == "") {
    QString idString = QString::number(sceneNodes.size() + 1);
    node->setName("Node" + idString.toStdString());
  } else if (sceneNodes.count(node->getName()) > 0) {
    QString idString = QString::number(sceneNodes.size() + 1);
    node->setName(node->getName() + idString.toStdString());
  }
  LogInfo << "Loading Node"<< node->getName();
  sceneNodes.insert(node->getName(), node);
}

void SceneGraph::setPosition(string nodeName, const QVector3D& position) {
  getNode(nodeName)->setPosition(position);
}

//void SceneGraph::printMatrix(const QMatrix4x4 & matrix, string name) {
//  Logger::Instance().message << name;
//  for (int i = 0; i < 16; i++) {
//    Logger::Instance().message << matrix.data()[i] << "\t";
//    if (i%4 == 3) Logger::Instance().message << "\n";
//  }
//  Logger::Instance().log("Print Matrix");
//}

void SceneGraph::setShadowCoords(Node * node, DirectionNode * viewPoint) {
  //TODO: Multiple lights
    QMatrix4x4 camViewToShadowMapMatrix = bias
            * SceneData::Instance().getShadowLight()->getProjection()
            * SceneData::Instance().getShadowLight()->getView()
            * viewPoint->getView().inverted();

    node->getMaterial()->getShaderProgram()->setUniform(
        "camViewToShadowMapMatrix", camViewToShadowMapMatrix);
}

void SceneGraph::drawNodes() {
  drawNodes(SceneData::Instance().getCurrentCamera());
}

void SceneGraph::drawNodes(DirectionNode * viewPoint) {
  QMap <qreal, Node*> transparentNodes;

//  QVector3D terrainPositon(SceneData::Instance().getCurrentCamera()->position.x(),-50,SceneData::Instance().getCurrentCamera()->position.z());
//  sceneNodes["terrain"]->setPosition(terrainPositon);

//  SceneData::Instance().updateLightBuffer();

  //TODO: Multiple lights
    foreach(Node * node, sceneNodes) {
        if(!node->transparent) {
          node->setView(viewPoint);
          setShadowCoords(node, viewPoint);
          SceneData::Instance().getShadowLight()->bindShaderUpdate(node->getMaterial()->getShaderProgram());
          node->draw();
        } else {
          QVector3D distance = node->getCenter() - SceneData::Instance().getCurrentCamera()->position;
          transparentNodes.insert(distance.length(), node);
        }
    }
    if (transparentNodes.size() > 0) {
      glEnable(GL_BLEND);
      QList<qreal> transparentKeys = transparentNodes.keys();
      qSort(transparentKeys.begin(), transparentKeys.end(), qGreater<qreal>());

      foreach(qreal transparentKey, transparentKeys) {
        Node * node = transparentNodes[transparentKey];
        node->setView(viewPoint);
        node->draw();
      }
      glDisable(GL_BLEND);
    }
    glError;
}

void SceneGraph::drawCasters(Material * material, DirectionNode * viewPoint) {
    foreach(Node * node, sceneNodes) {
        if (node->getCastShadows()) {
          node->setView(material->getShaderProgram(), viewPoint);
          node->mesh->draw();
        }
    }
    glError;
}

void SceneGraph::meshCube(const QList<string> & attributes, string file, float cubeSize, float step,
        Material * material) {
    Mesh * mesh = MeshLoader::load(attributes, file);

    for (float x = -cubeSize / 2.0; x < cubeSize / 2.0; x += step) {
        for (float y = -cubeSize / 2.0; y < cubeSize / 2.0; y += step) {
            for (float z = -cubeSize / 2.0; z < cubeSize / 2.0; z += step) {
                addNode(new Node("", { x, y, z }, 1, mesh, material));
            }
        }
    }
}

void SceneGraph::meshCube(const QList<string> & attributes, string file, float cubeSize, float step,
    vector<Material*> materials) {
    Mesh * mesh = MeshLoader::load(attributes, file);
    unsigned position = 0;
    for (float x = -cubeSize / 2.0; x < cubeSize / 2.0; x += step) {
        for (float y = -cubeSize / 2.0; y < cubeSize / 2.0; y += step) {
            for (float z = -cubeSize / 2.0; z < cubeSize / 2.0; z += step) {
                addNode(new Node("", { z, x, y }, 1, mesh,
                    materials.at(position % materials.size())));
                position++;
            }
        }
    }
}

void SceneGraph::meshPlane(Mesh * mesh, float cubeSize, float step,
    vector<Material*> materials) {
    unsigned position = 0;
    for (float x = -cubeSize / 2.0; x < cubeSize / 2.0; x += step) {
        for (float y = -cubeSize / 2.0; y < cubeSize / 2.0; y += step) {
            addNode(new Node("", { x, y, -5 + x }, 1, mesh,
                materials.at(position % materials.size())));
            position++;
        }
    }
}
