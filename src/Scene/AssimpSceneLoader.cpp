#include "AssimpSceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Scene/Scene.h"
#include "System/Config.h"
#include "Material/Shaders.h"
#include "Material/Textures.h"
#include "Mesh/MeshLoader.h"
#include "System/Timer.h"


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
    Mesh * mesh = MeshLoader::getMeshFromAssimp(assMesh, attributes, true);
    return mesh;
}

void AssimpSceneLoader::printColor(const string & name, const aiColor4D & color) {
    LogDebug << name
             << color.r
             << color.g
             << color.b
             << color.a;
}

void AssimpSceneLoader::load(string file) {
    Timer * timer = new Timer();
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
            Material * newMaterial = new Material(materialName.data);
            LogDebug << "Material" << materialName.data << ":" << material->mNumProperties << "Properties";

            aiColor4D diffuseColor;
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
            printColor("diffuseColor", diffuseColor);
            newMaterial->diffuseColor = Material::colorFromAssimp(diffuseColor);

            aiColor4D specularColor;
            material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
            printColor("specularColor", specularColor);
            newMaterial->specularColor = Material::colorFromAssimp(specularColor);

            float shininess;
            material->Get(AI_MATKEY_SHININESS, shininess);
            LogDebug << "shininess" << shininess;
            newMaterial->shininess = shininess;

            aiColor4D ambientColor;
            material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
            printColor("ambientColor", ambientColor);
            newMaterial->ambientColor = Material::colorFromAssimp(ambientColor);

            aiColor4D reflectiveColor;
            material->Get(AI_MATKEY_COLOR_REFLECTIVE, reflectiveColor);
            printColor("reflectiveColor", reflectiveColor);
            newMaterial->reflectiveColor = Material::colorFromAssimp(reflectiveColor);

            unsigned texC = material->GetTextureCount(aiTextureType_DIFFUSE);
            LogDebug << texC << "textures";

            for (unsigned texIndex = 0; texIndex < texC; texIndex++) {
                aiString textureName;
                material->Get(AI_MATKEY_TEXTURE_DIFFUSE(texIndex), textureName);
                string texturePath = textureName.data;

                QString blenderDir = QString::fromStdString(texturePath);
                blenderDir.replace("//textures", QString::fromStdString(Config::Instance().value<string> ("meshDir")+ "textures"));
                texturePath = blenderDir.toStdString();

                string targetName = "Texture";
                if (texIndex < 1)
                    targetName = "diffuseTexture";
                else if (texIndex < 2)
                    targetName = "normalTexture";

                Texture * texture = new TextureFile(texturePath, targetName);
                newMaterial->addTexture(texture);
            }

            materials.push_back(newMaterial);
        }
    } else {
        LogError << "Scene Has no Materials.";
    }
    initNode(assimpScene->mRootNode);

    importer.FreeScene();

    timer->frameDone();

    LogWarning << "Scene Loading took" << timer->getSeconds() << "secounds";
}
