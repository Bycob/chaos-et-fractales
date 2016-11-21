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
    int size = image.spectrum();
    unsigned char * pixels = new unsigned char[width * height * size];

    for (int x = 0 ; x < width ; x++) {
        for (int y = 0 ; y < height ; y++) {
            if (size >= 1) {
                pixels[(x * height + y) * size] = image(y, x, 0, 0);
            }
            if (size >= 3) {
                pixels[(x * height + y) * size + 1] = image(y, x, 0, 1);
                pixels[(x * height + y) * size + 2] = image(y, x, 0, 2);
            }
            if (size >= 4) {
                pixels[(x * height + y) * size + 3] = image(y, x, 0, 3);
            }
        }
    }

    GLint format = size == 3 ? GL_RGB : (size == 4 ? GL_RGBA : GL_RGBA); //TODO complèter
    Texture result(width, height, pixels, format, format);

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