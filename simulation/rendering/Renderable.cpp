//
// Created by louis on 07/10/16.
//
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Renderable.h"
#include "Context.h"
#include "Texture.h"
#include "Program.h"


Renderable::Renderable() {
    
}

Renderable::~Renderable() {
    deleteBuffers();
}

bool Renderable::shouldRender() {
    return this->active;
}

void Renderable::regenerateBuffers() {
    deleteBuffers();

    glGenVertexArrays(1, &this->gVAO);

    glGenBuffers(1, &this->vertexBuffer);
    glGenBuffers(1, &this->normalBuffer);
    glGenBuffers(1, &this->texCoordBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(this->gVAO);
}

void Renderable::deleteBuffers() {
    if (this->gVAO != 0) {
        glDeleteVertexArrays(1, &this->gVAO);
        this->gVAO = 0;
    }

    if (this->vertexBuffer != 0) {
        glDeleteBuffers(1, &this->vertexBuffer);
        this->vertexBuffer = 0;
    }

    if (this->normalBuffer != 0) {
        glDeleteBuffers(1, &this->normalBuffer);
        this->normalBuffer = 0;
    }

    if (this->texCoordBuffer != 0) {
        glDeleteBuffers(1, &this->texCoordBuffer);
        this->texCoordBuffer = 0;
    }
}




// -----


RenderableModel::RenderableModel() : Renderable() {

}

RenderableModel::~RenderableModel() {

}

void RenderableModel::setPosition(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

glm::vec3 RenderableModel::getPosition() {
    return glm::vec3(this->x, this->y, this->z);
}

void RenderableModel::setMaterial(Material &material) {
    this->material = material;
    this->compiled = false;
}

void RenderableModel::addTexturePath(std::string path) {
    this->texturePaths.push_back(path);
    this->compiled = false;
}

void RenderableModel::loadTextures() {
    this->material.removeAllTextures();

    for (const std::string & path : this->texturePaths) {
        try {
            this->material.addTexture(Texture::load(path));
        }
        catch (std::runtime_error & e) {
            std::cerr << "Texture non chargée : " << path << std::endl;
        }
    }
}

