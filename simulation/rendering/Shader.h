//
// Created by louis on 08/10/16.
//

#ifndef RENDUOPENGL_SHADER_H
#define RENDUOPENGL_SHADER_H

#include <glad/glad.h>
#include <string>
#include <memory>

class Shader {
public :
    static Shader loadFromFile(std::string filename, GLenum shaderType);

    Shader(std::string &shaderCode, GLenum shaderType);
    Shader(const Shader &other);
    virtual ~Shader();

    int getID() const {return id;}
    GLenum getShaderType() const {return shaderType;}
private :

    int id;
    GLenum shaderType;

    std::shared_ptr<int> refCount;
};


#endif //RENDUOPENGL_SHADER_H
