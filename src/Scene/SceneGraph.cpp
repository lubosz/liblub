/*
 * Copyright © 2010 Lubosz Sarnecki
 */
#include <vector>
#include <string>
#include "Scene/SceneGraph.h"
#include "Scene/Camera.h"
#include "Renderer/RenderEngine.h"
#include "Material/Materials.h"
#include "Mesh/MeshFactory.h"

SceneGraph::SceneGraph() {
    bias = QMatrix4x4();
    bias.translate(.5, .5, .5);
    bias.scale(.5, .5, .5);
}

void SceneGraph::animate(float frameCount) {
    /*
     rotate(modelmatrix, (GLfloat) frameCount * -1.0, X_AXIS);
     rotate(modelmatrix, (GLfloat) frameCount * 1.0, Y_AXIS);
     rotate(modelmatrix, (GLfloat) frameCount * 0.5, Z_AXIS);
     */
}

void SceneGraph::setPosition(string nodeName, const QVector3D& position) {
    foreach(Node *node, sceneNodes) {
        if (node->getName() == nodeName) {
            node->setPosition(position);
        }
    }
}

void SceneGraph::printMatrix(const QMatrix4x4 & matrix, string name) {
  Logger::Instance().message << name;
  for (int i = 0; i < 16; i++) {
    Logger::Instance().message << matrix.data()[i] << "\t";
    if (i%4 == 3) Logger::Instance().message << "\n";
  }
  Logger::Instance().log("Print Matrix");
}

void SceneGraph::setShadowCoords(Node * node, DirectionNode * viewPoint) {
    QMatrix4x4 camViewToShadowMapMatrix = bias
            * SceneGraph::Instance().light->getProjection()
            * SceneGraph::Instance().light->getView()
            * viewPoint->getView().inverted();

    node->getMaterial()->getShaderProgram()->setUniform(
            camViewToShadowMapMatrix, "camViewToShadowMapMatrix");
}

void SceneGraph::drawNodes(DirectionNode * viewPoint) {
    foreach(Node * node, sceneNodes) {
        node->bindShaders(viewPoint);
        // TODO(bmonkey): wtf
        if (true) {
            setShadowCoords(node, viewPoint);
        }
        light->bindShaderUpdate(node->getMaterial()->getShaderProgram());
        node->draw();
    }
    glError("SceneGraph", 61);
}

void SceneGraph::drawCasters(Material * material) {
    foreach(Node * node, sceneNodes) {
        if (node->getCastShadows()) {
            node->bindShaders(material->getShaderProgram(), light);
            light->bindShaderUpdate(material->getShaderProgram());
            node->mesh->draw();
        }
    }
    glError("SceneGraph", 74);
}

void SceneGraph::addNode(Node * node) {
    sceneNodes.push_back(node);
}

void SceneGraph::meshCube(string file, float cubeSize, float step,
        Material * material) {
    Mesh * mesh = MeshFactory::load(file);

    for (float x = -cubeSize / 2.0; x < cubeSize / 2.0; x += step) {
        for (float y = -cubeSize / 2.0; y < cubeSize / 2.0; y += step) {
            for (float z = -cubeSize / 2.0; z < cubeSize / 2.0; z += step) {
                addNode(new Node("", { x, y, z }, 1, mesh, material));
            }
        }
    }
}

void SceneGraph::meshCube(string file, float cubeSize, float step,
    vector<Material*> materials) {
    Mesh * mesh = MeshFactory::load(file);
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
