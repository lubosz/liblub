#ifndef AssimpSceneLoader_H
#define AssimpSceneLoader_H

#include <string>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "common/Singleton.h"
#include "Mesh/Mesh.h"
#include "Material/ShaderProgram.h"
#include "Scene/Material.h"

using std::string;

class AssimpSceneLoader : public Singleton<AssimpSceneLoader>
{
public:
    vector<Mesh*> meshes;
    const aiScene* assimpScene;
    vector<Material*> materials;

    AssimpSceneLoader();
    void load(string file);
    Mesh * initMesh(aiMesh * assMesh);
    void initNode(aiNode * parent);
    void printColor(const string &name, const aiColor4D & color);
};

#endif // AssimpSceneLoader_H
