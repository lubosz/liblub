#include "BlenderLoader.h"
#include "Scene/SceneGraph.h"
#include "Scene/Scene.h"
#include "System/Config.h"
#include "Material/Shaders.h"
#include "Material/Textures.h"
#include "Mesh/MeshLoader.h"

BlenderLoader::BlenderLoader() {
}

QMatrix4x4 BlenderLoader::qFromAssimpMatrix(const aiMatrix4x4 &aMatrix) {

    QMatrix4x4 qMatrix =
    //                QMatrix4x4();
    //            QMatrix4x4(aMatrix.a1,aMatrix.a2,aMatrix.a3,aMatrix.a4,
    //                       aMatrix.b1,aMatrix.b2,aMatrix.b3,aMatrix.b4,
    //                       aMatrix.c1,aMatrix.c2,aMatrix.c3,aMatrix.c4,
    //                       aMatrix.d1,aMatrix.d2,aMatrix.d3,aMatrix.d4);
            QMatrix4x4(aMatrix.a1,aMatrix.b1,aMatrix.c1,aMatrix.d1,
                       aMatrix.a2,aMatrix.b2,aMatrix.c2,aMatrix.d2,
                       aMatrix.a3,aMatrix.b3,aMatrix.c3,aMatrix.d3,
                       aMatrix.a4,aMatrix.b4,aMatrix.c4,aMatrix.d4);
    //            qaMatrix = qaMatrix.transposed();
    //            LogDebug << "Rot"<<rotation.x<<rotation.y<<rotation.z;
    return qMatrix;
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

//        QMatrix4x4 transQ;
//        QTransform tranlationo = transQ.toTransform();

//        tranlationo.

        aiMatrix4x4 rotMatrix = aiMatrix4x4(rotation.GetMatrix());
        QMatrix4x4 theNewRotMatrix;
        QQuaternion qquat =
//                QQuaternion(rotation.x, rotation.y, rotation.z, rotation.w);
                QQuaternion(rotation.w, rotation.x, rotation.y, rotation.z);
        theNewRotMatrix.rotate(qquat);
        theNewRotMatrix = theNewRotMatrix.inverted();
        theNewRotMatrix = theNewRotMatrix.transposed();
//            someMeshNode->setRotation(QVector3D(rotation.x,rotation.y,rotation.z));
//        someMeshNode->setRotation(qFromAssimpMatrix(rotMatrix));
        someMeshNode->setRotation(theNewRotMatrix);

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
    QList<string> attributes = QList<string> () << "uv" << "normal" << "tangent" << "bitangent";
    Mesh * mesh = MeshLoader::getMeshFromAssimp(assMesh, attributes);
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
