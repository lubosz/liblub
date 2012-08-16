#include "AssimpSceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Scene/Scene.h"
#include "System/Config.h"
#include "Material/Shaders.h"
#include "Material/Textures.h"
#include "Mesh/MeshLoader.h"

AssimpSceneLoader::AssimpSceneLoader() {
}

void AssimpSceneLoader::initNode(aiNode * parent) {

    aiMatrix4x4 trans = parent->mTransformation;

    aiVector3D scaling;
    aiQuaternion rotation;
    aiVector3D position;

    trans.Decompose(scaling, rotation, position);

    QVector3D qposition =
            QVector3D(position.y, position.z, position.x);

    for (unsigned i = 0; i < parent->mNumMeshes; i++) {
        unsigned meshId = parent->mMeshes[i];
        aiMesh * mesh = assimpScene->mMeshes[meshId];

        Node * someMeshNode = new Node(
                    parent->mName.data,
                    qposition,
                    scaling.x,
                    meshes[meshId],
                    materials[mesh->mMaterialIndex]);

//        someMeshNode->transparent = true;

        QQuaternion qrotation(rotation.w, rotation.y, rotation.z, rotation.x);
        someMeshNode->setRotation(qrotation);
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

Mesh * AssimpSceneLoader::initMesh(aiMesh * assMesh) {
    QList<string> attributes = QList<string> () << "uv" << "normal" << "tangent" << "bitangent";
    Mesh * mesh = MeshLoader::getMeshFromAssimp(assMesh, attributes);
    return mesh;
}

void AssimpSceneLoader::load(string file) {
    LogDebug << "Loading" << file;

    string path = Config::Instance().value<string> ("meshDir") + file;

    Assimp::Importer importer;

    assimpScene = importer.ReadFile(path,
         aiProcess_CalcTangentSpace
       | aiProcess_Triangulate);

    if (!assimpScene) {
        LogError << "Assimp Scene Load"<< importer.GetErrorString();
    }

    LogInfo << assimpScene->mNumCameras << "cameras.";
    LogInfo << assimpScene->mNumLights << "lights.";
    LogInfo << assimpScene->mNumAnimations << "animations.";
    LogInfo << assimpScene->mNumMaterials << "materials.";
    LogInfo << assimpScene->mNumTextures << "textures.";
    LogInfo << assimpScene->mNumMeshes<< "meshes.";

    if (!assimpScene->HasLights()) {
        LogWarning << "Assimp didn't find Lights.";
    }

    if (!assimpScene->HasTextures()) {
        LogWarning << "Assimp didn't find Textures.";
    }

    if (assimpScene->HasMeshes()) {
        for (unsigned i = 0; i < assimpScene->mNumMeshes; i++)
            meshes.push_back(initMesh(assimpScene->mMeshes[i]));
    } else {
        LogError << "Scene" << path << "has no Meshes.";
    }

    if (assimpScene->HasMaterials()) {
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
