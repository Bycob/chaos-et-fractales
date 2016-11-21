//
// Created by louis on 08/10/16.
//

#ifndef RENDUOPENGL_PROGRAM_H
#define RENDUOPENGL_PROGRAM_H

#include <vector>
#include <glm/detail/type_mat.hpp>
#include "Shader.h"

class Program {
public :
    Program(std::vector<Shader>& shaders);
    Program(const Program &other);
    ~Program();

    void addShader(Shader & shader);
    void removeShader(Shader & shader);

    void use() const;
    bool isInUse() const;
    void stopUsing() const;

    GLuint attrib(std::string attribName);
    GLuint uniform(std::string uniformName);

    void setUniformMatrix4(std::string uniformName, glm::mat4 &matrix);
    void setUniformMatrix3(std::string uniformName, glm::mat3 &matrix);
    void setUniform3f(std::string uniformName, const GLfloat &x, const GLfloat &y, const GLfloat &z);
    void setUniform1i(std::string uniformName, const GLint &value);
    void setUniform1f(std::string uniformName, const GLfloat &value);
private :
    GLuint id;
};


#endif //RENDUOPENGL_PROGRAM_H
