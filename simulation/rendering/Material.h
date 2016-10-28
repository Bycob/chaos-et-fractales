//
// Created by louis on 22/10/16.
//

#ifndef RENDUOPENGL_MATERIAL_H
#define RENDUOPENGL_MATERIAL_H

#include "Texture.h"

class Context;

class Material {

public :
    Material(float diffuseR = 0.8f, float diffuseG = 0.8f, float diffuseB = 0.8f,
        float specularR = 1.0f, float specularG = 1.0f, float specularB = 1.0f,
        float ambientR = 0.2f, float ambientG = 0.2f, float ambientB = 0.2f);

    void setTexture(Texture texture);

    void setDiffuse(float r, float g, float b);

    void setSpecular(float r, float g, float b);
    void setSpecularParameters(float intensity, float hardness);

    void setAmbient(float r, float g, float b);

    void setEmit(bool emit) {
        this->emit = emit;
    }

    void pushMaterial(Context * context);

private :
    Texture texture;

    float diffuseR, diffuseG, diffuseB;
    float specularR, specularG, specularB;
    float ambientR, ambientG, ambientB;

    float specularIntensity;
    float specularHardness;

    bool emit;
};


#endif //RENDUOPENGL_MATERIAL_H
