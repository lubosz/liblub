/*
 * Copyright © 2010 Lubosz Sarnecki
 * MeshFactory.cpp
 *
 *  Created on: Jun 8, 2010
 */
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <string>
#include <vector>
#include "Mesh/MeshLoader.h"
#include "Mesh/Geometry.h"
#include "System/Config.h"
#include "System/Logger.h"
#include <QRectF>
#include "Renderer/OpenGL.h"

Mesh * MeshLoader::getMeshFromAssimp(aiMesh * assMesh, const QList<string> & attributes) {
    Mesh * mesh = new Mesh(attributes);
    mesh->name = assMesh->mName.data;

    for (unsigned i = 0; i < assMesh->mNumFaces; i++) {
        aiFace face = assMesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++) {
            int vertex = face.mIndices[j];

            aiVector3D position = assMesh->mVertices[vertex];
            mesh->vertex("position",position.y,position.z,position.x);
            mesh->getBoundingBox()->update(position.x,position.y,position.z);

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
}

Mesh * MeshLoader::load(const QList<string> & attributes, string file) {
    LogDebug << "Loading" << file;

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

    Mesh * mesh = getMeshFromAssimp(assMesh, attributes);

    glError;
//    delete assMesh;
    importer.FreeScene();
    return mesh;
}
