/*
 * Copyright © 2010 Lubosz Sarnecki
 * MengerSponge.cpp
 *
 *  Created on: Jul 11, 2010
 */

#include "Mesh/MengerSponge.h"
#include <vector>
#include <algorithm>

MengerSponge::MengerSponge(unsigned recursion) {
  oneMeshIndices = {};
  oneMeshVertices = {};
    mesh = new Mesh();

    makeSponge(recursion, {0, 0, 0}, 1.0f);
    mesh->addBuffer(oneMeshVertices, 3, "in_Vertex");
    mesh->addBuffer(oneMeshVertices, 3, "in_Normal");
    mesh->addBuffer(oneMeshVertices, 3, "in_Color");
    mesh->addElementBuffer(oneMeshIndices);
//        mesh->addBuffer(globalVertices, 3, "in_Vertex");
//        mesh->addBuffer(globalVertices, 3, "in_Normal");
//        mesh->addBuffer(globalVertices, 3, "in_Color");
//        mesh->addElementBuffer(globalIndices);

    mesh->setDrawType(GL_TRIANGLES);
}

MengerSponge::~MengerSponge() {
  // TODO(bmonkey): Auto-generated destructor stub
}

void MengerSponge::makeSponge(
        unsigned recursion, const vector<float> & position, float size) {
  if (recursion == 0) {
    addCube(position, size);
  } else {
    recursion-=1;
    size/=3.0f;
    float trans = size*2;

    // TOP
    makeSponge(recursion,
            {position[0], position[1]+trans, position[2]+trans}, size);

    makeSponge(recursion,
            {position[0]-trans, position[1]+trans, position[2]}, size);
    makeSponge(recursion,
            {position[0], position[1]+trans, position[2]-trans}, size);
    makeSponge(recursion,
            {position[0]+trans, position[1]+trans, position[2]}, size);


    makeSponge(recursion,
            {position[0]+trans, position[1]+trans, position[2]+trans},
            size);
    makeSponge(recursion,
            {position[0]+trans, position[1]+trans, position[2]-trans},
            size);

    makeSponge(recursion,
            {position[0]-trans, position[1]+trans, position[2]+trans},
            size);
    makeSponge(recursion,
            {position[0]-trans, position[1]+trans, position[2]-trans},
            size);

    // MIDDLE
    makeSponge(recursion,
            {position[0]+trans, position[1], position[2]+trans}, size);
    makeSponge(recursion,
            {position[0]+trans, position[1], position[2]-trans}, size);

    makeSponge(recursion,
            {position[0]-trans, position[1], position[2]+trans}, size);
    makeSponge(recursion,
            {position[0]-trans, position[1], position[2]-trans}, size);

    // BOTTOM
    makeSponge(recursion,
            {position[0], position[1]-trans, position[2]+trans}, size);
    makeSponge(recursion,
            {position[0], position[1]-trans, position[2]-trans}, size);

    makeSponge(recursion,
            {position[0]+trans, position[1]-trans, position[2]}, size);
    makeSponge(recursion,
            {position[0]-trans, position[1]-trans, position[2]}, size);

    makeSponge(recursion,
            {position[0]+trans, position[1]-trans, position[2]+trans},
            size);
    makeSponge(recursion,
            {position[0]+trans, position[1]-trans, position[2]-trans},
            size);

    makeSponge(recursion,
            {position[0]-trans, position[1]-trans, position[2]+trans},
            size);
    makeSponge(recursion,
            {position[0]-trans, position[1]-trans, position[2]-trans},
            size);
  }
}

void MengerSponge::addCube(const vector<float> & position, float size) {
  unsigned indexSize = oneMeshVertices.size()/3;

  vector<GLfloat> vertices = {
      size + position[0], -size + position[1], -size + position[2],
      size + position[0], -size + position[1], size + position[2],
      -size + position[0], -size + position[1], size + position[2],
      -size + position[0], -size + position[1], -size + position[2],
      size + position[0], size + position[1], -size + position[2],
      size + position[0], size + position[1], size + position[2],
      -size + position[0], size + position[1], size + position[2],
      -size + position[0], size + position[1], -size + position[2]
  };

  oneMeshVertices.insert(
          oneMeshVertices.end(), vertices.begin(), vertices.end());

//  vector<GLfloat> uvCoords = {
//      1.0, 0.0,
//      0.0, 0.0,
//      0.0, 1.0,
//      1.0, 1.0,
//      0.0, 0.0,
//      1.0, 0.0,
//      1.0, 1.0,
//      0.0, 1.0
//  };
//
//  oneMeshUVCoords.insert(
//          oneMeshUVCoords.end(), uvCoords.begin(), uvCoords.end());
//
//  vector<GLfloat> normals = {
//      1.0, -1.0, -1.0,
//      1.0, -1.0, 1.0,
//      -1.0, -1.0, 1.0,
//      -1.0, -1.0, -1.0,
//      1.0, 1.0, -1.0,
//      1.0, 1.0, 1.0,
//      -1.0, 1.0, 1.0,
//      -1.0, 1.0, -1.0
//  };
//
//  oneMeshNormals.insert(oneMeshNormals.end(), normals.begin(), normals.end());



  vector<GLuint> indices;

  vector<GLuint> back = {
                  // Back
                  4+indexSize, indexSize, 3+indexSize,
                  4+indexSize,  3+indexSize,  7+indexSize
  };

  indices.insert(indices.end(), back.begin(), back.end());

  vector<GLuint> front = {
                  // Front
                  1+indexSize, 5+indexSize, 2+indexSize,
                  5+indexSize, 6+indexSize, 2+indexSize
  };
  indices.insert(indices.end(), front.begin(), front.end());

  vector<GLuint> left = {
                  // Left
                  2+indexSize,  6+indexSize,  7+indexSize,
                  2+indexSize,  7+indexSize,  3+indexSize
  };
  indices.insert(indices.end(), left.begin(), left.end());

  vector<GLuint> right = {
                  // Right
                  indexSize, 4+indexSize, 1+indexSize,
                  4+indexSize, 5+indexSize, 1+indexSize
  };
  indices.insert(indices.end(), right.begin(), right.end());

  vector<GLuint> top = {
                  // Top
                  4+indexSize, 7+indexSize, 5+indexSize,
                  7+indexSize, 6+indexSize, 5+indexSize
  };
  indices.insert(indices.end(), top.begin(), top.end());

  vector<GLuint> bottom = {
                  // Bottom
                  indexSize, 1+indexSize, 2+indexSize,
                  indexSize, 2+indexSize, 3+indexSize
  };
  indices.insert(indices.end(), bottom.begin(), bottom.end());

  oneMeshIndices.insert(oneMeshIndices.end(), indices.begin(), indices.end());
}

Mesh * MengerSponge::getMesh() {
  return mesh;
}
