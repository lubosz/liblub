/*
 * Copyright © 2010 Lubosz Sarnecki
 */
#include <vector>
#include <string>
#include "Scene/SceneGraph.h"
#include "Scene/Camera.h"
#include "Renderer/OpenGL.h"
#include "Mesh/MeshLoader.h"
#include "System/Logger.h"
#include "Scene/Scene.h"
#include <cassert>
#include "Renderer/DeferredRenderer.h"

SceneGraph::SceneGraph() {
    bias = QMatrix4x4();
    bias.translate(.5, .5, .5);
    bias.scale(.5, .5, .5);
}

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
  LogDebug << "Loading Node"<< node->getName();
  sceneNodes.insert(node->getName(), node);
}

void SceneGraph::setPosition(string nodeName, const QVector3D& position) {
  getNode(nodeName)->setPosition(position);
}

void SceneGraph::drawNodes() {
  drawNodes(Scene::Instance().getCurrentCamera());
}

void SceneGraph::drawNodes(DirectionNode * viewPoint) {
  QMap <qreal, Node*> transparentNodes;

    foreach(Node * node, sceneNodes) {
        if(!node->getTransparent()) {
          node->setView(viewPoint);
          node->setShadowCoords(viewPoint);
          Scene::Instance().getShadowLight()->bindShaderUpdate(node->getShader());
          node->draw();
        } else {
          QVector3D distance = node->getCenter() - Scene::Instance().getCurrentCamera()->position;
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

void SceneGraph::drawNodes(ShaderProgram * shader) {
  drawNodes(shader, Scene::Instance().getCurrentCamera());
}

void SceneGraph::drawNodes(ShaderProgram * shader, DirectionNode * viewPoint) {
    shader->use();

    GLint MMatrix = shader->getUniformLocation("MMatrix");
    GLint MVMatrix = shader->getUniformLocation("MVMatrix");
    GLint NormalMatrix = shader->getUniformLocation("NormalMatrix");
    GLint MVPMatrix = shader->getUniformLocation("MVPMatrix");

    foreach(Node * node, sceneNodes) {
        //          node->setView(shader, viewPoint);
        QMatrix4x4 tempMatrix = viewPoint->getView() * node->modelMatrix;
        shader->setUniform(MMatrix, node->modelMatrix);
        shader->setUniform(MVMatrix, tempMatrix);
        shader->setUniform(NormalMatrix, tempMatrix.normalMatrix());
        tempMatrix = viewPoint->getProjection() * tempMatrix;
        shader->setUniform(MVPMatrix, tempMatrix);

        node->draw(shader);
    }
    glError;
}

void SceneGraph::drawReceivers(ShaderProgram * shader) {
  DirectionNode * camView = Scene::Instance().getCurrentCamera();
  Node::setShadowCoords(shader,camView);

  QMap <qreal, Node*> transparentNodes;

    foreach(Node * node, sceneNodes) {
        if(node->getMaterial() && !node->getMaterial()->transparent) {
            node->draw(shader, camView);
        } else {
          QVector3D distance = node->getCenter() - Scene::Instance().getCurrentCamera()->position;
          transparentNodes.insert(distance.length(), node);
        }
    }
    if (transparentNodes.size() > 0) {
        QList<qreal> transparentKeys = transparentNodes.keys();
        qSort(transparentKeys.begin(), transparentKeys.end(), qGreater<qreal>());

        if(DeferredRenderer::Instance().drawTransparency) {
            glEnable(GL_BLEND);
        }
      foreach(qreal transparentKey, transparentKeys) {
        transparentNodes[transparentKey]->draw(shader, camView);
      }
      if(DeferredRenderer::Instance().drawTransparency) {
          glDisable(GL_BLEND);
      }
    }
  glError;
}

void SceneGraph::drawCasters(ShaderProgram * shader) {
  drawCasters(shader, Scene::Instance().getCurrentCamera());
}

void SceneGraph::drawCasters(ShaderProgram * shader, DirectionNode * viewPoint) {
    shader->use();

    GLint MMatrix = shader->getUniformLocation("MMatrix");
    GLint MVMatrix = shader->getUniformLocation("MVMatrix");
    GLint NormalMatrix = shader->getUniformLocation("NormalMatrix");
    GLint MVPMatrix = shader->getUniformLocation("MVPMatrix");

    //TODO: uniforms
    foreach(Node * node, sceneNodes) {
        if (node->getCastShadows()) {
            //          node->setView(shader, viewPoint);
            QMatrix4x4 tempMatrix = viewPoint->getView() * node->modelMatrix;
            shader->setUniform(MMatrix, node->modelMatrix);
            shader->setUniform(MVMatrix, tempMatrix);
            shader->setUniform(NormalMatrix, tempMatrix.normalMatrix());
            tempMatrix = viewPoint->getProjection() * tempMatrix;
            shader->setUniform(MVPMatrix, tempMatrix);

            node->getMesh()->draw();
        }
    }
    glError;
}


void SceneGraph::meshCube(const QList<string> & attributes, string file, float cubeSize, float step,
        ShaderProgram * shader) {
    Mesh * mesh = MeshLoader::load(attributes, file);

    for (float x = -cubeSize / 2.0; x < cubeSize / 2.0; x += step) {
        for (float y = -cubeSize / 2.0; y < cubeSize / 2.0; y += step) {
            for (float z = -cubeSize / 2.0; z < cubeSize / 2.0; z += step) {
                addNode(new Node("Mesh Cube Node", QVector3D(x, y, z), 1, mesh, shader));
            }
        }
    }
}

void SceneGraph::meshCube(const QList<string> & attributes, string file, float cubeSize, float step,
    vector<ShaderProgram*> materials) {
    Mesh * mesh = MeshLoader::load(attributes, file);
    unsigned position = 0;
    for (float x = -cubeSize / 2.0; x < cubeSize / 2.0; x += step) {
        for (float y = -cubeSize / 2.0; y < cubeSize / 2.0; y += step) {
            for (float z = -cubeSize / 2.0; z < cubeSize / 2.0; z += step) {
                addNode(new Node("Mesh Cube Node", QVector3D(z, x, y), 1, mesh,
                    materials.at(position % materials.size())));
                position++;
            }
        }
    }
}

void SceneGraph::meshPlane(Mesh * mesh, float cubeSize, float step,
    vector<ShaderProgram*> materials) {
    unsigned position = 0;
    for (float x = -cubeSize / 2.0; x < cubeSize / 2.0; x += step) {
        for (float y = -cubeSize / 2.0; y < cubeSize / 2.0; y += step) {
            addNode(new Node("Mesh Plane Node", QVector3D(x, y, -5 + x), 1, mesh,
                materials.at(position % materials.size())));
            position++;
        }
    }
}
