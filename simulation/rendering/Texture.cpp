//
// Created by louis on 27/10/16.
//

#include <CImg/CImg.h>

#include "Texture.h"

Texture Texture::load(std::string path) {

    //TODO Gérer le cas où on arrive pas à trouver l'image.

    cimg_library::CImg<unsigned char> image = cimg_library::CImg<>(path.c_str()).normalize(0, 255);
    int width = image.width();
    int height = image.height();
    unsigned char * pixels = new unsigned char[width * height * 3];

    for (int x = 0 ; x < width ; x++) {
        for (int y = 0 ; y < height ; y++) {
            pixels[(x * height + y) * 3] = image(y, x, 0);
            pixels[(x * height + y) * 3 + 1] = image(y, x, 1);
            pixels[(x * height + y) * 3 + 2] = image(y, x, 2);
        }
    }

    Texture result(width, height, pixels, GL_RGB, GL_RGB);

    delete[] pixels;

    return result;
}

Texture::Texture(int width, int height, const unsigned char* pixel, GLint internalFormat, GLenum format) {

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixel);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    this->instance_count = std::make_shared<int>(1);
}

Texture::Texture(const Texture &other) {
    this->id = other.id;
    this->instance_count = other.instance_count;

    *this->instance_count += 1;
}

Texture::~Texture() {
    *this->instance_count -= 1;

    if (*this->instance_count ==0) {
        glDeleteTextures(1, &this->id);
    }
}