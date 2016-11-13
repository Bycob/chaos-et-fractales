//
// Created by louis on 17/10/16.
//

#ifndef RENDUOPENGL_CONTEXT_H
#define RENDUOPENGL_CONTEXT_H

#include <GLFW/glfw3.h>
#include <memory>
#include <map>

#include "Program.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

class Program;

class Context {

public :
    Context(GLFWwindow *window);
    void setWindow(GLFWwindow * window);
    void getWindowDimensions(int &width, int &height);

    void setup();

    void setLight(Light &light);
    void setMaterial(Material &material);
    void setCamera(Camera &camera);

    /** Charge le programme composé des shaders passés en paramètres.
     * @param vertexShader le chemin du vertex shader
     * @param fragmentShader le chemin du fragment shader*/
    void loadProgram(std::string name, std::string vertexShader, std::string fragmentShader);
    /** Change le programme actuel. Le programme est automatiquement
     * mis en mode utilisation. */
    void setCurrentProgram(std::string currentProgramID);
    bool setDefaultProgram(std::string programID);
    std::string getDefaultProgram() {return this->_defaultProgram;}
    Program& program() {return *_programMap.at(this->_currentProgram);}
private :
    GLFWwindow *_window;

    std::string _currentProgram;
    std::string _defaultProgram = "default";
    std::map<std::string, std::shared_ptr<Program>> _programMap;

    Material _material;
    Camera * _camera = nullptr;
    Light _light;
};


#endif //RENDUOPENGL_CONTEXT_H
