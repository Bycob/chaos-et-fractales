//
// Created by louis on 08/10/16.
//
// Librement inspiré du code de tom dalling
// https://github.com/tomdalling/opengl-series
//

#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Program.h"

Program::Program(std::vector<Shader> &shaders) {
    if(shaders.size() <= 0)
        throw std::runtime_error("No shaders were provided to create the program");

    //create the program object
    id = glCreateProgram();
    if(id == 0)
        throw std::runtime_error("glCreateProgram failed");

    //attach all the shaders
    for(unsigned i = 0; i < shaders.size(); ++i)
        glAttachShader(id, shaders[i].getID());

    //link the shaders together
    glLinkProgram(id);

    //detach all the shaders
    for(unsigned i = 0; i < shaders.size(); ++i)
        glDetachShader(id, shaders[i].getID());

    //throw exception if linking failed
    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Program linking failure: ");

        GLint infoLogLength;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(id, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteProgram(id); id = 0;
        throw std::runtime_error(msg);
    }
}

Program::Program(const Program &other) {
    throw new std::runtime_error("Program : copy not supported");
}

Program::~Program() {
    glDeleteProgram(id);
}

void Program::addShader(Shader &shader) {

}

void Program::removeShader(Shader &shader) {

}

void Program::use() const {
    glUseProgram(id);
}

bool Program::isInUse() const {
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return (currentProgram == (GLint) id);
}

void Program::stopUsing() const {
    glUseProgram(0);
}

GLint Program::attrib(const GLchar *attribName) {
    if(!attribName)
        throw std::runtime_error("Please give a non nullptr attribName");

    GLint attrib = glGetAttribLocation(id, attribName);

    return attrib;
}

GLint Program::uniform(const GLchar* uniformName) {
    if(!uniformName)
        throw std::runtime_error("Please give a non nullptr uniformName");

    GLint uniform = glGetUniformLocation(id, uniformName);

    return uniform;
}

void Program::setUniformMatrix4(GLchar * uniformName, glm::mat4 &matrix) {
    if (!isInUse()) throw std::runtime_error("This program is not in use.");
    glUniformMatrix4fv(uniform(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::setUniform3f(GLchar *uniformName, const GLfloat &x, const GLfloat &y, const GLfloat &z) {
    if (!isInUse()) throw std::runtime_error("This program is not in use.");
    glUniform3f(uniform(uniformName), x, y, z);
}

void Program::setUniform1i(GLchar *uniformName, const GLint &value) {
    if (!isInUse()) throw std::runtime_error("This program is not in use.");
    glUniform1i(uniform(uniformName), value);
}

void Program::setUniform1f(GLchar *uniformName, const GLfloat &value) {
    if (!isInUse()) throw std::runtime_error("This program is not in use.");
    glUniform1f(uniform(uniformName), value);
}
