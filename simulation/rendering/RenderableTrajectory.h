//
// Created by louis on 06/11/16.
//

#ifndef RENDUOPENGL_RENDERABLETRAJECTORY_H
#define RENDUOPENGL_RENDERABLETRAJECTORY_H

#include <glm/glm.hpp>
#include <list>
#include "Renderable.h"

class RenderableTrajectory : public Renderable {
public :
    RenderableTrajectory(int maxPointCount = 1000);

    void addPoint(float x, float y, float z);

    virtual void render(Context *context);
private :
    int maxPointCount;
    std::list<glm::vec3> points;

    void buildRenderData(Context *context);
};


#endif //RENDUOPENGL_RENDERABLETRAJECTORY_H
