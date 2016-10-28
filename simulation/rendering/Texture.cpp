//
// Created by louis on 27/10/16.
//

#include "Texture.h"

Texture Texture::load(std::string path) {
    Texture result;

    return result;
}

Texture::Texture() {

    this->instance_count = std::make_shared<int>(1);
}

Texture::Texture(const Texture &other) {
    this->id = other.id;
    this->instance_count = other.instance_count;

    *this->instance_count += 1;
}

Texture::~Texture() {
    *this->instance_count -= 1;
}