//
// Created by louis on 22/10/16.
//

#include <glad/glad.h>
#include <iostream>

#include "Material.h"
#include "Context.h"

Material::Material(float diffuseR, float diffuseG, float diffuseB, float specularR, float specularG, float specularB,
                    float ambientR, float ambientG, float ambientB) :

        ambientR(ambientR), ambientG(ambientG), ambientB(ambientB), diffuseR(diffuseR), diffuseG(diffuseG), diffuseB(diffuseB),
        specularR(specularR), specularG(specularG), specularB(specularB), specularIntensity(0.5), specularHardness(5) {

}

void Material::removeAllTextures() {
    this->textures.clear();
}

void Material::addTexture(Texture texture) {
    this->textures.push_back(texture);
}

void Material::setDiffuse(float r, float g, float b) {
    diffuseR = r;
    diffuseG = g;
    diffuseB = b;
}

void Material::setSpecular(float r, float g, float b) {
    specularR = r;
    specularG = g;
    specularB = b;
}

void Material::setSpecularParameters(float intensity, float hardness) {
    specularIntensity = intensity;
    specularHardness = hardness;
}

void Material::setAmbient(float r, float g, float b) {
    ambientR = r;
    ambientG = g;
    ambientB = b;
}

void Material::pushMaterial(Context * context) {
    glActiveTexture(GL_TEXTURE0);
    if (this->textures.size() >= 1) {
        context->program().setUniform1i("useTextures", 1);
        glBindTexture(GL_TEXTURE_2D, this->textures[0].getID());
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
        context->program().setUniform1i("useTextures", 0);
    }

    context->program().setUniform1f("material.alpha", alpha);

    context->program().setUniform3f("material.ambientColor", ambientR, ambientG, ambientB);
    context->program().setUniform3f("material.diffuseColor", diffuseR, diffuseG, diffuseB);
    context->program().setUniform3f("material.specularColor", specularR, specularG, specularB);

    context->program().setUniform1f("material.specularIntensity", specularIntensity);
    context->program().setUniform1f("material.specularHardness", specularHardness);

    context->program().setUniform1i("material.emit", emit);
}