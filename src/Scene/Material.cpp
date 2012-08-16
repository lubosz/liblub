#include "Material.h"

Material::Material(const string &name) : name (name), transparent(false) {
}

QVector4D Material::colorFromAssimp(aiColor4D assimpColor) {
    QVector4D qColor = QVector4D(assimpColor.r, assimpColor.g, assimpColor.b, assimpColor.a);
    return qColor;
}

bool Material::hasTexture() {
    return textures.size() > 0;
}

bool Material::hasNormalMap() {
    return textures.size() > 1;
}

void Material::activateAndBindTextures() {
    foreach(Texture* texture, textures) {
            texture->activate();
            texture->bind();
        }
    glError;
}

void Material::addTexture(Texture * texture) {
    if (textures.size() == 0)
        if (texture->glChannelOrder == GL_RGBA)
            transparent = true;
    textures.push_back(texture);
}

void Material::uniforms(ShaderProgram *shaderProgram) {
    shaderProgram->use();
    shaderProgram->setUniform("matDiffuseColor", diffuseColor);
    shaderProgram->setUniform("matSpecularColor", specularColor);
    shaderProgram->setUniform("matShininess", shininess);
    shaderProgram->setUniform("matAmbientColor", ambientColor);
    shaderProgram->setUniform("matReflectiveColor", reflectiveColor);
    shaderProgram->setUniform("matHasTexture", hasTexture());
    shaderProgram->setUniform("matIsTransparent", transparent);

    foreach(Texture* tex, textures)
        tex->uniform(shaderProgram->getHandle());
}
