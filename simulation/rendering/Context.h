//
// Created by louis on 17/10/16.
//

#ifndef RENDUOPENGL_CONTEXT_H
#define RENDUOPENGL_CONTEXT_H

#include <GLFW/glfw3.h>
#include <memory>
#include "Program.h"

class Program;
class Light;
class Material;

class Context {

public :
    Context(GLFWwindow *window);
    void setWindow(GLFWwindow * window);
    void getWindowDimensions(int &width, int &height);

    void pushLight(Light &light);
    void pushMaterial(Material &material);

    Program& program() {return *_program;}
private :
    GLFWwindow *_window;

    //TODO permettre à l'utilisateur de définir son propre programme ou de changer de programme en cours de route.
    std::unique_ptr<Program> _program = nullptr;
};


#endif //RENDUOPENGL_CONTEXT_H
