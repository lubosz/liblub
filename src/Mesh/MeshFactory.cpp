/*
 * Copyright © 2010 Lubosz Sarnecki
 * MeshFactory.cpp
 *
 *  Created on: Jun 8, 2010
 */
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>
#include <string>
#include <vector>
#include "Mesh/MeshFactory.h"
#include "Mesh/Geometry.h"
#include "System/Config.h"
#include "System/Logger.h"
#include <QRectF>

Mesh * MeshFactory::load(string file) {
    string path = Config::Instance().value<string> ("meshDir") + file;

    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(path,
            aiProcess_CalcTangentSpace
            | aiProcess_Triangulate);
            // | aiProcess_JoinIdenticalVertices
            // | aiProcess_SortByPType

    // If the import failed, report it
    if (!scene) {
        Logger::Instance().log("ERROR",
                "Assimp Scene Load", importer.GetErrorString());
    }

    aiMesh * assMesh = scene->mMeshes[0];


    vector<GLfloat> positions;
    vector<GLfloat> normals;
    vector<GLfloat> tangents;
    vector<GLfloat> bitangents;
    vector<GLfloat> uvs;
    vector<GLuint> indices;

    QVector3D boundingBoxMin = QVector3D();
    QVector3D boundingBoxMax = QVector3D();

    unsigned numIndices = 0;

    for (unsigned i = 0; i < assMesh->mNumFaces; i++) {
        aiFace face = assMesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++) {
            int vertex = face.mIndices[j];

            aiVector3D position = assMesh->mVertices[vertex];
            positions.push_back(position.x);
            positions.push_back(position.y);
            positions.push_back(position.z);

            if(i == 0 && j == 0) {
                boundingBoxMin = QVector3D(position.x,position.y,position.z);
                boundingBoxMax = QVector3D(position.x,position.y,position.z);
            } else {
              if (boundingBoxMin.x() > position.x)
                boundingBoxMin.setX(position.x);
              if (boundingBoxMin.y() > position.y)
                boundingBoxMin.setY(position.y);
              if (boundingBoxMin.z() > position.z)
                boundingBoxMin.setZ(position.z);
              if (boundingBoxMax.x() < position.x)
                boundingBoxMax.setX(position.x);
              if (boundingBoxMax.y() < position.y)
                boundingBoxMax.setY(position.y);
              if (boundingBoxMax.z() < position.z)
                boundingBoxMax.setZ(position.z);
            }

            if(assMesh->HasNormals()) {
              aiVector3D normal = assMesh->mNormals[vertex];
              normals.push_back(normal.x);
              normals.push_back(normal.y);
              normals.push_back(normal.z);
            }

            if (assMesh->HasTangentsAndBitangents()) {
              aiVector3D tangent = assMesh->mTangents[vertex];
              tangents.push_back(tangent.x);
              tangents.push_back(tangent.y);
              tangents.push_back(tangent.z);

              aiVector3D bitangent = assMesh->mBitangents[vertex];

              bitangents.push_back(bitangent.x);
              bitangents.push_back(bitangent.y);
              bitangents.push_back(bitangent.z);
            }

            if(assMesh->HasTextureCoords(0)){
              aiVector3D uv = assMesh->mTextureCoords[0][vertex];
              uvs.push_back(uv.x);
              uvs.push_back(uv.y);
            }

            indices.push_back(numIndices);
            numIndices++;
        }
    }

    Mesh * mesh = new Mesh();

    mesh->init();

    mesh->addBuffer(positions, 3, "in_Vertex");

    if (assMesh->HasNormals())
      mesh->addBuffer(normals, 3, "in_Normal");
    else
      Logger::Instance().log("WARNING", "Assimp Scene Load", file + " has no Normals :/");

//    if (assMesh->HasTangentsAndBitangents()) {
//      mesh->addBuffer(tangents, 3, "in_Tangent");
//      mesh->addBuffer(bitangents, 3, "in_Bitangent");
//    } else {
//      Logger::Instance().log("WARNING", "Assimp Scene Load", file + " has no Tangents :/");
//    }

    if (assMesh->HasTextureCoords(0))
      mesh->addBuffer(uvs, 2, "in_Uv");
    else
      Logger::Instance().log("WARNING", "Assimp Scene Load", file + " has no UV coords :/");

    mesh->addElementBuffer(indices);
    mesh->setDrawType(GL_TRIANGLES);
    mesh->boundingBox = new AABB(boundingBoxMin,boundingBoxMax);
    glError;
    return mesh;
}
