#ifndef BLENDERLOADER_H
#define BLENDERLOADER_H

#include <string>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "common/Singleton.h"
#include "Mesh/Mesh.h"
#include "Material/ShaderProgram.h"

using std::string;

class BlenderLoader : public Singleton<BlenderLoader>
{
public:
    vector<Mesh*> meshes;
    const aiScene* assimpScene;
    vector<ShaderProgram*> materials;

    BlenderLoader();
    void load(string file);
    Mesh * initMesh(aiMesh * assMesh);
    void initNode(aiNode * parent);

    QMatrix4x4 qFromAssimpMatrix(const aiMatrix4x4 &aMatrix);

};

#endif // BLENDERLOADER_H
