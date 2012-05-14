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
#include "Renderer/OpenGL.h"

Mesh * MeshLoader::load(const QList<string> & attributes, string file) {
    LogDebug << "Loading" << file;
    Mesh * mesh = new Mesh(attributes);
    string path = Config::Instance().value<string> ("meshDir") + file;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
            aiProcess_CalcTangentSpace
            | aiProcess_Triangulate);
            // | aiProcess_JoinIdenticalVertices
            // | aiProcess_SortByPType

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

            if(assMesh->HasNormals() && attributes.contains("normal")) {
              aiVector3D normal = assMesh->mNormals[vertex];
              mesh->vertex("normal",normal.x, normal.y, normal.z);
            }

            if (assMesh->HasTangentsAndBitangents() && attributes.contains("tangent")) {
              aiVector3D tangent = assMesh->mTangents[vertex];
              mesh->vertex("tangent",tangent.x, tangent.y, tangent.z);
            }

            if (assMesh->HasTangentsAndBitangents() && attributes.contains("bitangent")) {
              aiVector3D bitangent = assMesh->mBitangents[vertex];
              mesh->vertex("bitangent",bitangent.x, bitangent.y, bitangent.z);
            }

            if(assMesh->HasTextureCoords(0) && attributes.contains("uv")){
              aiVector3D uv = assMesh->mTextureCoords[0][vertex];
              mesh->vertex("uv",uv.x, uv.y);
            }
        }
    }

    mesh->init();
    mesh->setDrawType(GL_TRIANGLES);
    mesh->boundingBox = new AABB(boundingBoxMin,boundingBoxMax);
    glError;
//    delete assMesh;
    importer.FreeScene();
    return mesh;
}
