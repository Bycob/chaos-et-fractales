//
// Created by louis on 21/11/16.
//

#include <glm/gtx/matrix_transform_2d.hpp>
#include <iostream>

#include "Sprite.h"
#include "Context.h"

Sprite::Sprite(const std::string &texture, int width, int height) :
        _textureName(texture), _width(width), _height(height) {


}

void Sprite::render(Context *context, Scene *scene) {
    drawAt(context, 0, 0);
}

void Sprite::drawAt(Context *context, int x, int y) {
    if (!shouldRender()) return;

    context->setCurrentProgram("2D");

    if (!this->compiled) buildRenderData(context);

    //Matrice de translation
    auto model = glm::translate(glm::mat3(), glm::vec2(x, y));
    context->program().setUniformMatrix3("model", model);

    glBindTexture(GL_TEXTURE_2D, this->_texture->getID());

    glBindVertexArray(this->gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite::buildRenderData(Context *context) {
    float vertices[] = {
            0,      0,
            _width, 0,
            _width, _height,
            0,      0,
            0,      _height,
            _width, _height
    };

    float texCoords[] = {
            0, 0,
            1, 0,
            1, 1,
            0, 0,
            0, 1,
            1, 1
    };

    regenerateBuffers();

    //Vertices
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(context->program().attrib("vert"));
    glVertexAttribPointer(context->program().attrib("vert"), 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    //Tex coords
    glBindBuffer(GL_ARRAY_BUFFER, this->texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), texCoords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(context->program().attrib("vertTexCoord"));
    glVertexAttribPointer(context->program().attrib("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Chargement de la texture
    if (this->_texture == nullptr) {
        Texture loaded = Texture::load(_textureName);
        this->_texture = std::unique_ptr<Texture>(&loaded);
    }

    this->compiled = true;
}