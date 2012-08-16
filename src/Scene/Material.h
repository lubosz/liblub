#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <string>
#include <QVector4D>
#include <assimp/scene.h>
#include "Material/Texture.h"
#include "Material/ShaderProgram.h"

using std::vector;

class Material
{
public:
    Material(const string & name);

private:
    string name;

public:
    QVector4D diffuseColor;
    QVector4D specularColor;
    float shininess;
    QVector4D ambientColor;
    QVector4D reflectiveColor;
    bool transparent;


    bool hasTexture();
    bool hasNormalMap();

private:
    vector<Texture*> textures;
public:
    static QVector4D colorFromAssimp(aiColor4D assimpColor);

    void activateAndBindTextures();

    void uniforms(ShaderProgram * shaderProgram);

    void addTexture(Texture * texture);
};

#endif // MATERIAL_H
