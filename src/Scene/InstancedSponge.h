/*
 * InstancedSponge.h
 *
 *  Created on: Sep 11, 2011
 *      Author: bmonkey
 */

#ifndef INSTANCEDSPONGE_H_
#define INSTANCEDSPONGE_H_

#include "Mesh/Mesh.h"
#include "Material/UniformBuffer.h"
#include "Scene/Node.h"
#include <vector>
#include <QtCore>

class InstancedSponge : public Node{
public:
    vector<QVector4D> positions;
    unsigned positionBufferDataSize;
    vector<UniformBuffer *> positionBuffers;
    unsigned bufferCount;

    InstancedSponge(unsigned recursion, QList<string>& attributes);
    void makeSponge(unsigned recursion, const vector<float> & position, float size);
    void addCube(const vector<float> & position, float size);
    void draw(ShaderProgram * material);
    void initBuffers(ShaderProgram * material);
    UniformBuffer * initPositionBuffer(vector<QVector4D> * positionBufferData);
    void calculateBufferSize();
};
#endif /* INSTANCEDSPONGE_H_ */
