//
// Created by louis on 08/11/16.
//

#include <iostream>

#include "Window.h"
#include "Context.h"


void glfwErrorCallback(int error, const char* description) {
    std::cerr << "There was a glfw error : " << description << std::endl;
}



Window::Window() {
    if (!glfwInit()) {
        std::cerr << "Erreur au lancement de glfw." << std::endl;
        return;
    }

    glfwSetErrorCallback(glfwErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_SAMPLES, 16);
    _window = glfwCreateWindow(800, 600, "Simulation", NULL, NULL);

    if (!_window) {
        std::cerr << "Erreur au lancement de glfw." << std::endl;
        return;
    }

    glfwMakeContextCurrent(_window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    //Création du contexte
    this->_context = std::make_unique<Context>(_window);

    glEnable(GL_MULTISAMPLE);
}

Window::~Window() {
    destroy();
}

void Window::destroy() {
    glfwDestroyWindow(_window);
    glfwTerminate(); //~~~~~~~
}

int Window::shouldClose() {
    return glfwWindowShouldClose(_window);
}

void Window::setKeyCallback(GLFWkeyfun func) {
    glfwSetKeyCallback(_window, func);
}

void Window::setScrollCallback(GLFWscrollfun func) {
    glfwSetScrollCallback(_window, func);
}

void Window::setupFrame() {
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::finalizeFrame() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Window::select() {
    if (!isSelected()) {
        glfwMakeContextCurrent(_window);
    }
}

bool Window::isSelected() {
    return _window == glfwGetCurrentContext();
}




