//
// Created by louis on 27/10/16.
//

#include <opencv2/opencv.hpp>

#include "Texture.h"

std::map<std::string, Texture*> Texture::pathToIDMap;

Texture Texture::load(const std::string & path) {

    //Si l'image est déjà dans la table, quelle aubaine !
    if (pathToIDMap.find(path) != pathToIDMap.end()) {
        Texture* alreadyLoaded(pathToIDMap[path]);
        return Texture(*alreadyLoaded);
    }

    //Lecture de l'image
    cv::Mat imageBGR;
    imageBGR = cv::imread(path, 1);

    if (!imageBGR.data) {
        throw std::runtime_error("image not found : " + path);
    }

    int width = imageBGR.cols;
    int height = imageBGR.rows;
    int size = imageBGR.channels();

    // openCV charge l'image en BGR, on la veut en RGB
    if (size == 3) {
        cv::cvtColor(imageBGR, imageBGR, CV_BGR2RGB);
    }
    else if (size == 4) {
        cv::cvtColor(imageBGR, imageBGR, CV_BGRA2RGBA);
    }

    auto data = imageBGR.data;

    //Détermination du format openGL
    GLint internalFormat = size == 3 ? GL_RGB : (size == 4 ? GL_RGBA : 0); //TODO complèter

    Texture result(width, height, data, internalFormat, internalFormat);
    result.path = path;

    //Enregistrement de la texture dans la map, pour ne pas la recharger inutilement
    pathToIDMap[path] = new Texture(result);

    return result;
}

void Texture::clearCache() {
    for (auto texture : pathToIDMap) {
        delete texture.second;
    }

    pathToIDMap.clear();
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
    this->path = other.path;
}

Texture::~Texture() {
    if (this->instance_count.use_count() == 1) {
        glDeleteTextures(1, &this->id);
        this->instance_count.reset();
    }
}