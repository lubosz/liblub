#include "BlenderLoader.h"
#include "Scene/SceneGraph.h"
#include "Scene/Scene.h"
#include "System/Config.h"
#include "Material/Shaders.h"
#include "Material/Textures.h"

BlenderLoader::BlenderLoader()
{
}

void BlenderLoader::initNode(aiNode * parent) {

    aiMatrix4x4 trans = parent->mTransformation;

    aiVector3D scaling;
    aiQuaternion rotation;
    aiVector3D position;

    trans.Decompose(scaling, rotation, position);

    QVector3D qposition = QVector3D(position.x, position.z, position.y);

    for (unsigned i = 0; i < parent->mNumMeshes; i++) {
        unsigned meshId = parent->mMeshes[i];
        aiMesh * mesh = assimpScene->mMeshes[meshId];
//            LogDebug << prefix + "-" << mesh->mName.data << meshId;

        Node * someMeshNode = new Node(
                    parent->mName.data,
                    qposition,
                    scaling.x,
                    meshes[meshId],
                    materials[mesh->mMaterialIndex]);

        someMeshNode->transparent = true;

//            someMeshNode->setRotation(QVector3D(rotation.x,rotation.y,rotation.z));
        aiMatrix4x4 rotMatrix = aiMatrix4x4(rotation.GetMatrix());
        QMatrix4x4 qRotMatrix =
//                QMatrix4x4(0,0,0,0,
//                           0,0,0,0,
//                           0,0,0,0,
//                           0,0,0,0);
//            QMatrix4x4(rotMatrix.a1,rotMatrix.a2,rotMatrix.a3,rotMatrix.a4,
//                       rotMatrix.b1,rotMatrix.b2,rotMatrix.b3,rotMatrix.b4,
//                       rotMatrix.c1,rotMatrix.c2,rotMatrix.c3,rotMatrix.c4,
//                       rotMatrix.d1,rotMatrix.d2,rotMatrix.d3,rotMatrix.d4);
                QMatrix4x4(rotMatrix.a1,rotMatrix.b1,rotMatrix.c1,rotMatrix.d1,
                           rotMatrix.a2,rotMatrix.b2,rotMatrix.c2,rotMatrix.d2,
                           rotMatrix.a3,rotMatrix.b3,rotMatrix.c3,rotMatrix.d3,
                           rotMatrix.a4,rotMatrix.b4,rotMatrix.c4,rotMatrix.d4);
//            qRotMatrix = qRotMatrix.transposed();
//            LogDebug << "Rot"<<rotation.x<<rotation.y<<rotation.z;
        someMeshNode->setRotation(qRotMatrix);
        SceneGraph::Instance().addNode(someMeshNode);
    }

    QString lightName = parent->mName.data;
    if (lightName.contains("Lamp")) {
        LogDebug << "Found Light "<< lightName.toStdString() << "At" << position.x << position.y << position.z;
        Light * light = new Light(qposition, QVector3D());
        Scene::Instance().addLight(lightName.toStdString(), light);
    }

    for (unsigned i = 0; i < parent->mNumChildren; i++) {
        initNode(parent->mChildren[i]);
    }
}

Mesh * BlenderLoader::initMesh(aiMesh * assMesh) {

    QVector3D boundingBoxMin = QVector3D();
    QVector3D boundingBoxMax = QVector3D();

    QList<string> attributes = QList<string> () << "uv" << "normal" << "tangent" << "bitangent";


    Mesh * mesh = new Mesh(attributes);
    mesh->name = assMesh->mName.data;
//        LogDebug << mesh->name;

    for (unsigned i = 0; i < assMesh->mNumFaces; i++) {
        aiFace face = assMesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++) {
            int vertex = face.mIndices[j];

            aiVector3D position = assMesh->mVertices[vertex];
            mesh->vertex("position",position.x,position.z,position.y);

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
    return mesh;
}

void BlenderLoader::load(string file) {
    LogDebug << "Loading" << file;

    string path = Config::Instance().value<string> ("meshDir") + file;

    Assimp::Importer importer;

    assimpScene = importer.ReadFile(path,
         aiProcess_CalcTangentSpace
       | aiProcess_Triangulate);
    // | aiProcess_JoinIdenticalVertices
    // | aiProcess_SortByPType

    if (!assimpScene) {
        LogError << "Assimp Scene Load"<< importer.GetErrorString();
    }

    LogInfo << assimpScene->mNumCameras << "cameras.";
    LogInfo << assimpScene->mNumLights << "lights.";
    LogInfo << assimpScene->mNumAnimations << "animations.";
    LogInfo << assimpScene->mNumMaterials << "materials.";
    LogInfo << assimpScene->mNumTextures << "textures.";
    LogInfo << assimpScene->mNumMeshes<< "meshes.";

    if (assimpScene->HasLights()) {
        LogInfo << "Scene Has Lights";
    } else {
        LogError << "Scene Has no Lights.";
    }

    if (assimpScene->HasTextures()) {
        LogInfo << "Scene Has Textures";
    } else {
        LogError << "Scene Has no Textures.";
    }

    if (assimpScene->HasMeshes()) {
        for (unsigned i = 0; i < assimpScene->mNumMeshes; i++)
            meshes.push_back(initMesh(assimpScene->mMeshes[i]));
    } else {
        LogError << "Scene" << path << "has no Meshes.";
    }

    if (assimpScene->HasMaterials()) {
        LogInfo << "Scene Has Materials";

        for(unsigned i = 0; i < assimpScene->mNumMaterials; i++) {
            aiMaterial * material = assimpScene->mMaterials[i];

            aiString materialName;
            material->Get(AI_MATKEY_NAME, materialName);
            LogDebug << "Material" << materialName.data;

            aiColor4D diffuseColor;
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
            LogDebug << "Color" << diffuseColor.r << diffuseColor.g << diffuseColor.b << diffuseColor.a;

            aiString textureName;
            material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), textureName);
            string diffuseTexture = textureName.data;

            material->Get(AI_MATKEY_TEXTURE_DIFFUSE(1), textureName);
            string normalTexture = textureName.data;

            bool validNormal = normalTexture!= "" && normalTexture != diffuseTexture;

            ShaderProgram* mat;

            QList<string> attributes = QList<string> () << "normal" << "uv";
            if (diffuseTexture == "") {
                mat = new SimpleProgram("Color/PhongColor", attributes);
            } else {
                LogDebug << "Diffuse Texture" << diffuseTexture;
                Texture * matTexture = new TextureFile(diffuseTexture, "diffuseTexture");
                Texture * normalTex;
                if (validNormal) {
                    LogDebug << "Normal Texture" << normalTexture;
                    normalTex = new TextureFile(normalTexture, "normalTexture");
                }

                mat = new SimpleProgram("Texture/texture", attributes);
                mat->addTexture(matTexture);
                if (validNormal)
                     mat->addTexture(normalTex);
                mat->samplerUniforms();
            }

            materials.push_back(mat);
        }
    } else {
        LogError << "Scene Has no Materials.";
    }
    initNode(assimpScene->mRootNode);

    importer.FreeScene();
}
