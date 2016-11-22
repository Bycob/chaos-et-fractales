//
// Created by louis on 27/10/16.
//

#include <opencv2/opencv.hpp>

#include "Texture.h"

Texture Texture::load(const std::string & path) {

    //TODO Gérer le cas où on arrive pas à trouver l'image.

    cv::Mat imageBGR;
    imageBGR = cv::imread(path, 1);

    if (!imageBGR.data) {
        throw std::runtime_error("image not found : " + path);
    }

    int width = imageBGR.cols;
    int height = imageBGR.rows;
    int size = imageBGR.channels();

    if (size == 3) {
        cv::cvtColor(imageBGR, imageBGR, CV_BGR2RGB);
    }
    else if (size == 4) {
        cv::cvtColor(imageBGR, imageBGR, CV_BGRA2RGBA);
    }

    auto data = imageBGR.data;

    GLint internalFormat = size == 3 ? GL_RGB : (size == 4 ? GL_RGBA : 0); //TODO complèter
    Texture result(width, height, data, internalFormat, internalFormat);

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