//
// Created by louis on 06/11/16.
//

#include "RenderableTrajectory.h"

RenderableTrajectory::RenderableTrajectory(int maxPointCount) : maxPointCount(maxPointCount) {

}

void RenderableTrajectory::addPoint(float x, float y, float z) {
    points.push_back(glm::vec3(x, y, z));

    while (points.size() > maxPointCount) {
        points.pop_front();
    }

    this->compiled = false;
}

void RenderableTrajectory::render(Context *context) {
    if (!this->compiled) buildRenderData(context);

}

void RenderableTrajectory::buildRenderData(Context *context) {
    
}