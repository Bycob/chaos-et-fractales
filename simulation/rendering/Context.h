//
// Created by louis on 17/10/16.
//

#ifndef RENDUOPENGL_CONTEXT_H
#define RENDUOPENGL_CONTEXT_H

#include <GLFW/glfw3.h>
#include <memory>
#include <map>
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

    /** Charge le programme composé des shaders passés en paramètres.
     * @param vertexShader le chemin du vertex shader
     * @param fragmentShader le chemin du fragment shader*/
    void loadProgram(std::string name, std::string vertexShader, std::string fragmentShader);
    void setCurrentProgram(std::string currentProgramID);
    Program& program() {return *_programMap.at(this->_currentProgram);}
private :
    GLFWwindow *_window;

    std::string _currentProgram;
    std::map<std::string, std::shared_ptr<Program>> _programMap;
};


#endif //RENDUOPENGL_CONTEXT_H
