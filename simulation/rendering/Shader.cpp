//
// Created by louis on 08/10/16.
//

#include <fstream>
#include <iostream>
#include "Shader.h"

Shader Shader::loadFromFile(std::string filename, GLenum shaderType) {
    std::ifstream fileStream(filename);

    std::string shaderCode = "";
    std::string line;

    if (fileStream.is_open()) {
        while (getline(fileStream, line)) {
            shaderCode += line + "\n";
        }
        fileStream.close();
    }
    else throw std::runtime_error("Unable to read the stream.");

    return Shader(shaderCode, shaderType);
}

Shader::Shader(std::string& shaderCode, GLenum shaderType) : id(0), shaderType(shaderType) {

    id = glCreateShader(shaderType);

    const char* source = shaderCode.c_str();
    glShaderSource(id, 1, &source, nullptr);

    glCompileShader(id);

    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader:\n");

        GLint infoLogLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(id, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteShader(id); id = 0;
        throw std::runtime_error(msg);
    }

    refCount = std::make_shared<int>(1);
}

Shader::Shader(const Shader &other) {
    this->id = other.getID();
    this->refCount = other.refCount;
    *this->refCount += 1;
}

Shader::~Shader() {
    *this->refCount -= 1;
    if (*this->refCount == 0) {
        glDeleteShader(id);
    }
}
