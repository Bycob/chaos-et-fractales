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
#include "RenderableSphere.h"

#define SPHERE_MAP_RESOLUTION 50

Scene::Scene() : _camera(), _light(), _sphereMap(nullptr) {

}

void Scene::render(Context *context) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //Paramètrage du contexte
    int width, height;
    context->getWindowDimensions(width, height);

    context->program().use();

    this->_camera.setRatio((float) width / height);
    this->_camera.updateCamera();

    context->setCamera(_camera);
    context->setLight(_light);

    //Rendu de la spheremap
    if (this->_sphereMap != nullptr) {
        glDepthMask(GL_FALSE);
        this->_sphereMap->render(context, this);
        glDepthMask(GL_TRUE);
    }

    //Rendu de tous les objets
    for (auto object : objects) {
        context->setCurrentProgram("");
        object->render(context, this);
    }

    context->program().stopUsing();
}

void Scene::setLight(Light &light) {
    this->_light = light;
}

void Scene::setSphereMap(std::string texturePath) {
    if (texturePath == "") {
        this->_sphereMap = nullptr;
    }
    else {
        this->_sphereMap = std::make_unique<RenderableSphere>(0.5, SPHERE_MAP_RESOLUTION, SPHERE_MAP_RESOLUTION);
        this->_sphereMap->addTexturePath(texturePath);
        this->_sphereMap->setSpecialShader("cubemap");
    }
}

void Scene::addObject(std::shared_ptr<Renderable> renderable) {
    // Tri à l'insertion
    auto index = objects.begin();
    for ( ; index != objects.end() ; ++index) {
        std::shared_ptr<Renderable> inplace = *index;

        if (inplace->usesBlendMode() && !renderable->usesBlendMode()) {
            break;
        }
    }

    // Ajout de l'objet à la place trouvée par le tri.
    objects.insert(index, std::shared_ptr<Renderable>(renderable));
}
