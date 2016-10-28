//
// Created by louis on 17/10/16.
//

#include <vector>
#include <glad/glad.h>

#include "Context.h"
#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "Program.h"


Context::Context(GLFWwindow *window) : _window(window) {
    std::vector<Shader> shaders;
    shaders.push_back(Shader::loadFromFile("shaders/default_vertex_shader.glslv", GL_VERTEX_SHADER));
    shaders.push_back(Shader::loadFromFile("shaders/default_fragment_shader.glslf", GL_FRAGMENT_SHADER));
    _program = std::make_unique<Program>(shaders);
}


void Context::setWindow(GLFWwindow *window) {
    this->_window = window;
}

void Context::getWindowDimensions(int &width, int &height) {
    glfwGetFramebufferSize(this->_window, &width, &height);
}

void Context::pushLight(Light &light) {
    light.pushLight(this);
}

void Context::pushMaterial(Material &material) {
    material.pushMaterial(this);
}