/*
 * TreeSponge.cpp
 *
 *  Created on: Sep 11, 2011
 *      Author: bmonkey
 */

#include "TreeSponge.h"
#include "Load/MeshLoader.h"
#include "Scene/Scene.h"

TreeSponge::TreeSponge(unsigned recursion, QList<string>& attributes, ShaderProgram * shader) : Node("treesponge", QVector3D(), 1, MeshLoader::load(attributes, "cube.obj"), shader){
    maxRecursion = recursion;
    minRecursion = 0;
    setCastShadows(false);
}

TreeSponge::~TreeSponge() {
}

void TreeSponge::makeSponge(unsigned recursion, const QVector3D & cubePosition,
        float size) {
    // check if should subdivide by camera distance
    if (recursion > 0 && endRecursionAdaptive(recursion, cubePosition, size))
        recursion = 0;

    // end recursion
    if (recursion == 0) {
        modelMatrix.setToIdentity();
        modelMatrix.translate(cubePosition);
        modelMatrix.scale(size);

        QMatrix4x4 tempMatrix = viewProjection * modelMatrix;
        //              shaderProgram->setUniform("MMatrix", modelMatrix);
        //              shaderProgram->setUniform("MVMatrix", tempMatrix);
        //              shaderProgram->setUniform("NormalMatrix", tempMatrix.normalMatrix());

        if (isNotCulled(tempMatrix)) {
            shader->use();
            shader->setUniform("MVPMatrix", tempMatrix);
            mesh->draw();
        }
    // subdivide
    } else {
        recursion -= 1;
        size /= 3.0f;

        //if (recursion > 2 && recursion < maxRecursion - 1) {
        if(0) {
            drawAllChildrenSorted(recursion, cubePosition, size);
        } else {
            drawAllChildren(recursion, cubePosition, size);
        }
    }
}

bool TreeSponge::endRecursionAdaptive(unsigned recursion,
        const QVector3D & cubePosition, float size) {
    float halfSize = size / 2.0;
    QVector3D cubeCenter = cubePosition + QVector3D(halfSize, halfSize,
            halfSize);
    QVector3D distance = viewPoint->getPosition() - cubeCenter;
    float recursiveNess = 1.0 - float(recursion) / float(maxRecursion);
    if (distance.length() * recursiveNess > 0.3
            && maxRecursion - recursion >= minRecursion)
        return true;
    else
        return false;
}

bool TreeSponge::isNotCulled(const QMatrix4x4& mvp) {
    QVector3D minPosition = mvp * QVector3D(-1, -1, -1);
    QVector3D maxPosition = mvp * QVector3D(1, 1, 1);

    //faster:
//    float x = mvp(0,0)+mvp(1,0)+mvp(2,0);

//    QVector3D maxPosition2 = QVector3D(
//          mvp(0,0)+mvp(1,0)+mvp(2,0),
//          mvp(0,0)+mvp(0,1)+mvp(0,2),
//          mvp(3,0)+mvp(3,1)+mvp(3,2));

//    LogInfo << x << maxPosition2.y() << maxPosition2.x() << maxPosition.x();

    if ((minPosition.x() > -1 && minPosition.y() > -1 && minPosition.z() > -1)
            || (maxPosition.x() < 1 && maxPosition.y() < 1 && maxPosition.z()
                    < 1)) {
        // Draw Cube
        return true;
    } else {
        return false;
    }
}

void TreeSponge::drawAllChildren(unsigned recursion,
        const QVector3D& parentPosition, float size) {
    float trans = size * 2 * 1.1;
    foreach (QVector3D translation, translations) {
        makeSponge(recursion, translation * trans + parentPosition, size);
    }
}

void TreeSponge::drawAllChildrenSorted(unsigned recursion,
        const QVector3D& parentPosition, float size) {
    DirectionNode * viewPoint = Scene::Instance().getCurrentCamera();
    float trans = size * 2 * 1.1;
    QMap<qreal, QVector3D> distanceMap;

    foreach (QVector3D translation, translations) {
        QVector3D distance = viewPoint->getPosition() - translation * trans
                    + parentPosition;
        distanceMap.insert(distance.length(),
                    translation * trans + parentPosition);
    }

    QList<qreal> distanceKeys = distanceMap.keys();
    qSort(distanceKeys.begin(), distanceKeys.end(), qGreater<qreal> ());

    unsigned foo = 0;
    foreach(qreal distanceKey, distanceKeys) {
        makeSponge(recursion, distanceMap[distanceKey], size);
        foo++;
        if (foo >= 10)
            return;
    }
}

void TreeSponge::draw(ShaderProgram * material) {
    viewPoint = Scene::Instance().getCurrentCamera();
    viewProjection = viewPoint->getProjection() * viewPoint->getView();
    material->activateAndBindTextures();
    makeSponge(maxRecursion, QVector3D(0, 0, 0), 1.0f);
}
