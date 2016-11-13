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

    context->setCamera(_camera);
    context->setLight(_light);

    for (auto object : objects) {
        context->setCurrentProgram("");
        object->render(context, this);
    }

    context->program().stopUsing();
}

void Scene::setLight(Light &light) {
    this->_light = light;
}

void Scene::addObject(std::shared_ptr<Renderable> renderable) {
    auto index = objects.begin();
    for ( ; index != objects.end() ; ++index) {
        std::shared_ptr<Renderable> inplace = *index;

        if (inplace->usesBlendMode() && !renderable->usesBlendMode()) {
            break;
        }
    }

    objects.insert(index, std::shared_ptr<Renderable>(renderable));
}
