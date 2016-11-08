//
// Created by louis on 08/11/16.
//

#ifndef RENDUOPENGL_WINDOW_H
#define RENDUOPENGL_WINDOW_H

#include <bits/unique_ptr.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Context;

//TODO permettre multiwindowing (glfwInit et glfwTerminate)


class Window {
public :
    Window();
    virtual ~Window();

    void destroy();
    int shouldClose();

    void setKeyCallback(GLFWkeyfun func);
    void setScrollCallback(GLFWscrollfun func);

    void select();
    bool isSelected();

    void setupFrame();
    void finalizeFrame();

    Context * context() {return _context.get();};
    GLFWwindow * const window() {return _window;};
private :
    GLFWwindow * _window;
    bool _destroyed = false; //TODO throw error everywhere if destroyed

    std::unique_ptr<Context> _context;
};

#endif //RENDUOPENGL_WINDOW_H
