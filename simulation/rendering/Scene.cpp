//
// Created by louis on 07/10/16.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Scene.h"
#include "Renderable.h"
#include "Context.h"
#include "Camera.h"
#include "Program.h"

Scene::Scene() : _camera(), _light() {

}

void Scene::render(Context *context) {
    int width, height;
    context->getWindowDimensions(width, height);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    context->program().use();

    this->_camera.setRatio((float) width / height);
    this->_camera.updateCamera();
    this->_camera.setCameraView(context);

    for (auto object : objects) {
        //TODO voir à quel point on rétablit une configuration par défaut entre chaque rendu.
        context->setCurrentProgram("");
        this->_camera.setCameraView(context);
        object->render(context, this);
    }
    context->pushLight(_light);

    context->program().stopUsing();
}

void Scene::setLight(Light &light) {
    this->_light = light;
}

void Scene::addObject(std::shared_ptr<Renderable> renderable) {
    objects.push_back(std::shared_ptr<Renderable>(renderable));
}
