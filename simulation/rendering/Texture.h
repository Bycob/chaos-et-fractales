//
// Created by louis on 27/10/16.
//

#ifndef RENDUOPENGL_TEXTURE_H
#define RENDUOPENGL_TEXTURE_H

#include <glad/glad.h>
#include <memory>


class Texture {

public :

    static Texture load(const std::string & path);

    Texture(int width, int height, const unsigned char* pixel,
            GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA);
    Texture(const Texture & other);
    ~Texture();

    GLuint getID() {return this->id;}
private :
    //static std::map<std::string, GLuint> pathToIDMap; //TODO next thing todo

    GLuint id = 0;
    std::shared_ptr<int> instance_count;
};


#endif //RENDUOPENGL_TEXTURE_H
