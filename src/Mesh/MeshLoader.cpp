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

Mesh * MeshLoader::getMeshFromAssimp(aiMesh * assMesh, const QList<string> & attributes, bool changePositionOrder) {
    Mesh * mesh = new Mesh(attributes);

    mesh->name = assMesh->mName.data;

    vector<GLfloat> * positions = &mesh->buffers["position"];
    vector<GLfloat> * normals = &mesh->buffers["normal"];
    vector<GLfloat> * tangents = &mesh->buffers["tangent"];
    vector<GLfloat> * bitangents = &mesh->buffers["bitangent"];
    vector<GLfloat> * uvs = &mesh->buffers["uv"];

    bool useNormal = assMesh->HasNormals() && attributes.contains("normal");
    bool useTangent = assMesh->HasTangentsAndBitangents() && attributes.contains("tangent");
    bool useBiTangent = assMesh->HasTangentsAndBitangents() && attributes.contains("bitangent");
    bool useUv = assMesh->HasTextureCoords(0) && attributes.contains("uv");

    for (unsigned i = 0; i < assMesh->mNumFaces; i++) {
        aiFace face = assMesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++) {
            int vertex = face.mIndices[j];

            aiVector3D position = assMesh->mVertices[vertex];
            if (changePositionOrder)
                mesh->vertex(positions, position.y,position.z,position.x);
            else
                mesh->vertex(positions, position.x,position.y,position.z);

            mesh->getBoundingBox()->update(position.x,position.y,position.z);

            if (useNormal) {
              aiVector3D normal = assMesh->mNormals[vertex];
              mesh->vertex(normals, normal.x, normal.y, normal.z);
            }

            if (useTangent) {
              aiVector3D tangent = assMesh->mTangents[vertex];
              mesh->vertex(tangents, tangent.x, tangent.y, tangent.z);
            }

            if (useBiTangent) {
              aiVector3D bitangent = assMesh->mBitangents[vertex];
              mesh->vertex(bitangents, bitangent.x, bitangent.y, bitangent.z);
            }

            if(useUv){
              aiVector3D uv = assMesh->mTextureCoords[0][vertex];
              mesh->vertex(uvs, uv.x, uv.y);
            }
        }
    }

    mesh->init();
    mesh->setDrawType(GL_TRIANGLES);

    return mesh;
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

    Mesh * mesh = getMeshFromAssimp(assMesh, attributes, false);

    glError;
    importer.FreeScene();
    return mesh;
}
