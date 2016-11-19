//
// Created by louis on 07/10/16.
//

#ifndef RENDUOPENGL_SCENE_H
#define RENDUOPENGL_SCENE_H

#include <vector>
#include <memory>

#include "Camera.h"
#include "Light.h"

class Camera;
class Light;
class Context;
class Renderable;
class RenderableSphere;

class Scene {

public :
    Scene();

    Camera& camera() {return _camera;}

    void setLight(Light &light);
    void setSphereMap(std::string texture);

    void addObject(std::shared_ptr<Renderable> renderable);

    void render(Context *context);
private :
    std::unique_ptr<RenderableSphere> _sphereMap;

    Camera _camera;
    Light _light;

    std::vector<std::shared_ptr<Renderable>> objects;
};


#endif //RENDUOPENGL_SCENE_H
