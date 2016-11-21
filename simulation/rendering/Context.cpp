//
// Created by louis on 17/10/16.
//

#include <vector>
#include <iostream>
#include <glad/glad.h>

#include "../utils.h"
#include "Context.h"
#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "Program.h"


Context::Context(GLFWwindow *window) : _window(window) {
    loadProgram("default", "shaders/default_vertex_shader.glslv", "shaders/default_fragment_shader.glslf");
    loadProgram("trajectory", "shaders/trajectory_vert_shader.glslv", "shaders/trajectory_frag_shader.glslf");
    loadProgram("cubemap", "shaders/cubemap_vertex_shader.glslv", "shaders/cubemap_fragment_shader.glslf");
    loadProgram("2D", "shaders/2D_vertex_shader.glslv", "shaders/2D_fragment_shader.glslf");

    setCurrentProgram("default");
}


void Context::setWindow(GLFWwindow *window) {
    this->_window = window;
}

void Context::getWindowDimensions(int &width, int &height) {
    glfwGetFramebufferSize(this->_window, &width, &height);
}

void Context::setup() {
    if (startsWith(_currentProgram, "2D")) {
        glm::mat3x3 model;
        program().setUniformMatrix3("model", model);

        int width, height;
        getWindowDimensions(width, height);
        program().setUniform1i("screen.width", width);
        program().setUniform1i("screen.height", height);
    }
    else {
        this->_light.pushLight(this);
        this->_material.pushMaterial(this);

        if (this->_camera != nullptr) {
            this->_camera->setCameraView(this);
        }

        glm::mat4x4 model;
        program().setUniformMatrix4("model", model);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Context::setLight(Light &light) {
    this->_light = light;
    light.pushLight(this);
}

void Context::setMaterial(Material &material) {
    this->_material = material;
    material.pushMaterial(this);
}

void Context::setCamera(Camera &camera) {
    //TODO résoudre le risque de segfault
    this->_camera = &camera;
    camera.setCameraView(this);
}

void Context::setCurrentProgram(std::string currentProgramID) {
    // Si le nom de programme est inconnu, alors on charge le programme par défaut
    if (_programMap.find(currentProgramID) == _programMap.end()) {
        currentProgramID = _defaultProgram;
    }

    // Si le programme est déjà bon, pas besoin de resetup
    if (currentProgramID == _currentProgram && program().isInUse()) {
        return;
    }

    this->_currentProgram = currentProgramID;
    this->program().use();
    this->setup();
}

bool Context::setDefaultProgram(std::string programID) {
    if (_programMap.find(programID) == _programMap.end()) {
        return false;
    }

    this->_defaultProgram = programID;
    return true;
}

void Context::loadProgram(std::string name, std::string vertexShader, std::string fragmentShader) {
    std::vector<Shader> shaders;
    shaders.push_back(Shader::loadFromFile(vertexShader, GL_VERTEX_SHADER));
    shaders.push_back(Shader::loadFromFile(fragmentShader, GL_FRAGMENT_SHADER));
    this->_programMap[name] = std::make_shared<Program>(shaders);
}
