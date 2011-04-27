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
#include "Mesh/MeshLoader.h"
#include "Mesh/Geometry.h"
#include "System/Config.h"
#include "System/Logger.h"
#include <QRectF>
#include "Renderer/RenderEngine.h"

Mesh * MeshLoader::load(string file) {
    Mesh * mesh = new Mesh();
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
        LogError << "Assimp Scene Load"<< importer.GetErrorString();
    }

    aiMesh * assMesh = scene->mMeshes[0];

    QVector3D boundingBoxMin = QVector3D();
    QVector3D boundingBoxMax = QVector3D();

    for (unsigned i = 0; i < assMesh->mNumFaces; i++) {
        aiFace face = assMesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++) {
            int vertex = face.mIndices[j];

            aiVector3D position = assMesh->mVertices[vertex];
            mesh->vertex("position",position.x,position.y,position.z);

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
              mesh->vertex("normal",normal.x, normal.y, normal.z);
            }

            if (assMesh->HasTangentsAndBitangents()) {
              aiVector3D tangent = assMesh->mTangents[vertex];
              mesh->vertex("tangent",tangent.x, tangent.y, tangent.z);

              aiVector3D bitangent = assMesh->mBitangents[vertex];
              mesh->vertex("bitangent",bitangent.x, bitangent.y, bitangent.z);
            }

            if(assMesh->HasTextureCoords(0)){
              aiVector3D uv = assMesh->mTextureCoords[0][vertex];
              mesh->vertex("uv",uv.x, uv.y);
            }
        }
    }


    mesh->init();
    mesh->setDrawType(GL_TRIANGLES);
    mesh->boundingBox = new AABB(boundingBoxMin,boundingBoxMax);
    glError;
    return mesh;
}
