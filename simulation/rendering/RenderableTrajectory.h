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

    virtual void render(Context *context, Scene *scene);
private :
    int maxPointCount;
    bool pointsUpdated;
    std::list<glm::vec3> points;

    /// met à jour le tableau de points
    void buildRenderData(Context *context);
    void updatePoints(Context *context);
};


#endif //RENDUOPENGL_RENDERABLETRAJECTORY_H
