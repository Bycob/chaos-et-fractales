//
// Created by louis on 06/11/16.
//

#ifndef RENDUOPENGL_RENDERABLETRAJECTORY_H
#define RENDUOPENGL_RENDERABLETRAJECTORY_H

#include <glm/glm.hpp>
#include <list>
#include "Renderable.h"

#define DEFAULT_MAX_POINT_COUNT 20000

class RenderableTrajectory : public Renderable {
public :
RenderableTrajectory(int maxPointCount = DEFAULT_MAX_POINT_COUNT);

    void setColor(float r, float g, float b);
    glm::vec3 getColor() {
        return this->color;
    }
    void setAlpha(float alpha);
    float getAlpha() {
        return this->alpha;
    }
    void addPoint(float x, float y, float z);
    void reset();

    virtual bool shouldRender();

    virtual void render(Context *context, Scene *scene);
private :
    int maxPointCount;
    bool pointsUpdated;
    std::list<glm::vec3> points;

    glm::vec3 color;
    float alpha = 1;

    GLuint tangentBuffer;

    virtual void regenerateBuffers();
    virtual void deleteBuffers();

    /// met à jour le tableau de points
    void buildRenderData(Context *context);
    void updatePoints(Context *context);
};


#endif //RENDUOPENGL_RENDERABLETRAJECTORY_H
