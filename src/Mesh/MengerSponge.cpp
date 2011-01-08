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

    GLfloat center [] = {0, 0, 0};
    makeSponge(recursion, center, 1.0f);
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

//void MengerSponge::makeSponge(
//        unsigned recursion, const vector<float> & position, float size) {
//  if (recursion == 0) {
//    addCube(position, size);
//  } else {
//    recursion-=1;
//    float trans = size*2/3.0f;
//
//    // TOP
//    makeSponge(recursion,
//            {position[0], position[1]+trans, position[2]+trans}, size/3.0);
//
//    makeSponge(recursion,
//            {position[0]-trans, position[1]+trans, position[2]}, size/3.0);
//    makeSponge(recursion,
//            {position[0], position[1]+trans, position[2]-trans}, size/3.0);
//    makeSponge(recursion,
//            {position[0]+trans, position[1]+trans, position[2]}, size/3.0);
//
//
//    makeSponge(recursion,
//            {position[0]+trans, position[1]+trans, position[2]+trans},
//            size/3.0);
//    makeSponge(recursion,
//            {position[0]+trans, position[1]+trans, position[2]-trans},
//            size/3.0);
//
//    makeSponge(recursion,
//            {position[0]-trans, position[1]+trans, position[2]+trans},
//            size/3.0);
//    makeSponge(recursion,
//            {position[0]-trans, position[1]+trans, position[2]-trans},
//            size/3.0);
//
//    // MIDDLE
//    makeSponge(recursion,
//            {position[0]+trans, position[1], position[2]+trans}, size/3.0);
//    makeSponge(recursion,
//            {position[0]+trans, position[1], position[2]-trans}, size/3.0);
//
//    makeSponge(recursion,
//            {position[0]-trans, position[1], position[2]+trans}, size/3.0);
//    makeSponge(recursion,
//            {position[0]-trans, position[1], position[2]-trans}, size/3.0);
//
//    // BOTTOM
//    makeSponge(recursion,
//            {position[0], position[1]-trans, position[2]+trans}, size/3.0);
//    makeSponge(recursion,
//            {position[0], position[1]-trans, position[2]-trans}, size/3.0);
//
//    makeSponge(recursion,
//            {position[0]+trans, position[1]-trans, position[2]}, size/3.0);
//    makeSponge(recursion,
//            {position[0]-trans, position[1]-trans, position[2]}, size/3.0);
//
//    makeSponge(recursion,
//            {position[0]+trans, position[1]-trans, position[2]+trans},
//            size/3.0);
//    makeSponge(recursion,
//            {position[0]+trans, position[1]-trans, position[2]-trans},
//            size/3.0);
//
//    makeSponge(recursion,
//            {position[0]-trans, position[1]-trans, position[2]+trans},
//            size/3.0);
//    makeSponge(recursion,
//            {position[0]-trans, position[1]-trans, position[2]-trans},
//            size/3.0);
//  }
//}

void MengerSponge::makeSponge(
        unsigned recursion, GLfloat position[], GLfloat size) {
  if (recursion == 0) {
    addCube(position, size);
  } else {
    recursion-=1;
    GLfloat trans = size*2/3.0f;
    size = size /3.0;
    GLfloat newpos[3];

    // TOP
    newpos = {position[0], position[1]+trans, position[2]+trans};
    makeSponge(recursion,newpos, size);

    newpos = {position[0]-trans, position[1]+trans, position[2]};
    makeSponge(recursion,newpos, size);
    newpos = {position[0], position[1]+trans, position[2]-trans};
    makeSponge(recursion,newpos, size);
    newpos = {position[0]+trans, position[1]+trans, position[2]};
    makeSponge(recursion,newpos, size);


    newpos = {position[0]+trans, position[1]+trans, position[2]+trans};
    makeSponge(recursion,newpos, size);
    newpos = {position[0]+trans, position[1]+trans, position[2]-trans};
    makeSponge(recursion,newpos, size);

    newpos = {position[0]-trans, position[1]+trans, position[2]+trans};
    makeSponge(recursion,newpos, size);
    newpos = {position[0]-trans, position[1]+trans, position[2]-trans};
    makeSponge(recursion,newpos, size);

    // MIDDLE
    newpos = {position[0]+trans, position[1], position[2]+trans};
    makeSponge(recursion,newpos, size);
    newpos = {position[0]+trans, position[1], position[2]-trans};
    makeSponge(recursion,newpos, size);

    newpos = {position[0]-trans, position[1], position[2]+trans};
    makeSponge(recursion,newpos, size);
    newpos = {position[0]-trans, position[1], position[2]-trans};
    makeSponge(recursion,newpos, size);

    // BOTTOM
    newpos = {position[0], position[1]-trans, position[2]+trans};
    makeSponge(recursion,newpos, size);
    newpos = {position[0], position[1]-trans, position[2]-trans};
    makeSponge(recursion,newpos, size);

    newpos = {position[0]+trans, position[1]-trans, position[2]};
    makeSponge(recursion,newpos, size);
    newpos = {position[0]-trans, position[1]-trans, position[2]};
    makeSponge(recursion,newpos, size);

    newpos = {position[0]+trans, position[1]-trans, position[2]+trans};
    makeSponge(recursion,newpos, size);
    newpos = {position[0]+trans, position[1]-trans, position[2]-trans};
    makeSponge(recursion,newpos, size);

    newpos = {position[0]-trans, position[1]-trans, position[2]+trans};
    makeSponge(recursion,newpos, size);
    newpos = {position[0]-trans, position[1]-trans, position[2]-trans};
    makeSponge(recursion,newpos, size);
  }
}

//void MengerSponge::addCube(const vector<float> & position, float size) {
//  unsigned indexSize = oneMeshVertices.size()/3;
//
//  vector<GLfloat> vertices = {
//      size + position[0], -size + position[1], -size + position[2],
//      size + position[0], -size + position[1], size + position[2],
//      -size + position[0], -size + position[1], size + position[2],
//      -size + position[0], -size + position[1], -size + position[2],
//      size + position[0], size + position[1], -size + position[2],
//      size + position[0], size + position[1], size + position[2],
//      -size + position[0], size + position[1], size + position[2],
//      -size + position[0], size + position[1], -size + position[2]
//  };
//
//  oneMeshVertices.insert(
//          oneMeshVertices.end(), vertices.begin(), vertices.end());
//
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
//
//
//
//  vector<GLuint> indices;
//
//  vector<GLuint> back = {
//                  // Back
//                  4+indexSize, indexSize, 3+indexSize,
//                  4+indexSize,  3+indexSize,  7+indexSize
//  };
//
//  indices.insert(indices.end(), back.begin(), back.end());
//
//  vector<GLuint> front = {
//                  // Front
//                  1+indexSize, 5+indexSize, 2+indexSize,
//                  5+indexSize, 6+indexSize, 2+indexSize
//  };
//  indices.insert(indices.end(), front.begin(), front.end());
//
//  vector<GLuint> left = {
//                  // Left
//                  2+indexSize,  6+indexSize,  7+indexSize,
//                  2+indexSize,  7+indexSize,  3+indexSize
//  };
//  indices.insert(indices.end(), left.begin(), left.end());
//
//  vector<GLuint> right = {
//                  // Right
//                  indexSize, 4+indexSize, 1+indexSize,
//                  4+indexSize, 5+indexSize, 1+indexSize
//  };
//  indices.insert(indices.end(), right.begin(), right.end());
//
//  vector<GLuint> top = {
//                  // Top
//                  4+indexSize, 7+indexSize, 5+indexSize,
//                  7+indexSize, 6+indexSize, 5+indexSize
//  };
//  indices.insert(indices.end(), top.begin(), top.end());
//
//  vector<GLuint> bottom = {
//                  // Bottom
//                  indexSize, 1+indexSize, 2+indexSize,
//                  indexSize, 2+indexSize, 3+indexSize
//  };
//  indices.insert(indices.end(), bottom.begin(), bottom.end());
//
//  oneMeshIndices.insert(oneMeshIndices.end(), indices.begin(), indices.end());
//}

void MengerSponge::addCube(GLfloat position[], GLfloat size) {

  GLfloat vertices[] = {
      size + position[0], -size + position[1], -size + position[2],
      size + position[0], -size + position[1], size + position[2],
      -size + position[0], -size + position[1], size + position[2],
      -size + position[0], -size + position[1], -size + position[2],
      size + position[0], size + position[1], -size + position[2],
      size + position[0], size + position[1], size + position[2],
      -size + position[0], size + position[1], size + position[2],
      -size + position[0], size + position[1], -size + position[2]
  };

//  oneMeshVertices.resize(oneMeshVertices.size() +24);
//  copy ( vertices, vertices+24, oneMeshVertices.end() );


//  globalVertices = vertices;
//  unsigned localSize = sizeof( globalVertices ) / sizeof( globalVertices[0] );
//  GLfloat newVerts[globalSize+localSize];
//  for(int i = 0; i < globalSize; i++){
//    newVerts[i] = globalVertices[i];
//  }
  for(int i = 0; i < 24; i++)
    oneMeshVertices.push_back(vertices[i]);
//  free(globalVertices);
//  *globalVertices = *newVerts;
//
//  GLfloat normals[] = {
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
//  GLfloat newNormals[globalSize+localSize];
//  for(int i = 0; i < globalSize; i++){
//    newNormals[i] = globalNormals[i];
//  }
//  for(int i = 0; i < localSize; i++){
//    newNormals[globalSize+i] = normals[i];
//  }
//  free(globalNormals);
//  *globalNormals = *newNormals;

//  oneMeshNormals.insert(oneMeshNormals.end(), normals.begin(), normals.end());


//  unsigned indexSize = oneMeshVertices.size()/3;
GLuint indexSize = 0;
  GLuint indices[] = {

                  // Back
                  4+indexSize, indexSize, 3+indexSize,
                  4+indexSize,  3+indexSize,  7+indexSize,
                  // Front
                  1+indexSize, 5+indexSize, 2+indexSize,
                  5+indexSize, 6+indexSize, 2+indexSize,
                  // Left
                  2+indexSize,  6+indexSize,  7+indexSize,
                  2+indexSize,  7+indexSize,  3+indexSize,
                  // Right
                  indexSize, 4+indexSize, 1+indexSize,
                  4+indexSize, 5+indexSize, 1+indexSize,
                  // Top
                  4+indexSize, 7+indexSize, 5+indexSize,
                  7+indexSize, 6+indexSize, 5+indexSize,
                  // Bottom
                  indexSize, 1+indexSize, 2+indexSize,
                  indexSize, 2+indexSize, 3+indexSize
  };
  for(int i = 0; i < 36; i++)
    oneMeshIndices.push_back(indices[i]);

//  oneMeshIndices.resize(oneMeshIndices.size() +36);
//  copy ( vertices, vertices+36, oneMeshIndices.end() );
//  globalSize = sizeof( globalIndices ) / sizeof( globalIndices[0] );
//  localSize = sizeof( indices ) / sizeof( indices[0] );
//  GLfloat newIndices[globalSize+localSize];
//  for(int i = 0; i < globalSize; i++){
//    newIndices[i] = globalIndices[i];
//  }
//  for(int i = 0; i < localSize; i++){
//    newIndices[globalSize+i] = indices[i];
//  }
//  free(globalIndices);
//  *globalIndices = *newIndices;

//  oneMeshIndices.insert(oneMeshIndices.end(), indices.begin(), indices.end());
}

//void MengerSponge::makeCube() {
//  vector<GLfloat> vertices = {
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
//  vector<GLuint> indices;
//
//  vector<GLuint> back = {
//                  // Back
//                  4, 0, 3,
//                  4, 3, 7,
//  };
//
//  indices.insert(indices.end(), back.begin(), back.end());
//
//  vector<GLuint> front = {
//                  // Front
//                  1, 5, 2,
//                  5, 6, 2,
//  };
//  indices.insert(indices.end(), front.begin(), front.end());
//
//  vector<GLuint> left = {
//                  // Left
//                  2, 6, 7,
//                  2, 7, 3,
//  };
//  indices.insert(indices.end(), left.begin(), left.end());
//
//  vector<GLuint> right = {
//                  // Right
//                  0, 4, 1,
//                  4, 5, 1,
//  };
//  indices.insert(indices.end(), right.begin(), right.end());
//
//  vector<GLuint> top = {
//                  // Top
//                  4, 7, 5,
//                  7, 6, 5,
//  };
//  indices.insert(indices.end(), top.begin(), top.end());
//
//  vector<GLuint> bottom = {
//                  // Bottom
//                  0, 1, 2,
//                  0, 2, 3
//  };
//  indices.insert(indices.end(), bottom.begin(), bottom.end());
//
//
//  mesh = new Mesh();
//    mesh->addBuffer(vertices, 3, "in_Vertex");
//    mesh->addBuffer(vertices, 3, "in_Normal");
//    mesh->addBuffer(uvCoords, 2, "in_Uv");
//    mesh->addElementBuffer(indices);
//    mesh->setDrawType(GL_TRIANGLES);
//}

Mesh * MengerSponge::getMesh() {
  return mesh;
}
