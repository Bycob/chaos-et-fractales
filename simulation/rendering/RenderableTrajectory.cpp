//
// Created by louis on 06/11/16.
//

#include <iostream>

#include "RenderableTrajectory.h"
#include "Context.h"
#include "Scene.h"

RenderableTrajectory::RenderableTrajectory(int maxPointCount) : maxPointCount(maxPointCount) {

}

void RenderableTrajectory::addPoint(float x, float y, float z) {
    points.push_back(glm::vec3(x, y, z));

    while (points.size() > maxPointCount) {
        points.pop_front();
    }

    this->pointsUpdated = false;
}

void RenderableTrajectory::render(Context *context, Scene *scene) {
    if (this->points.size() == 0) return;

    if (!this->compiled) buildRenderData(context);
    if (!this->pointsUpdated) updatePoints(context);

    context->setCurrentProgram("trajectory");
    scene->camera().setCameraView(context);

    glBindVertexArray(this->gVAO);
    glDrawArrays(GL_LINES, 0, this->points.size());
    glBindVertexArray(0);
}

void RenderableTrajectory::buildRenderData(Context *context) {
    context->setCurrentProgram("trajectory");

    regenerateBuffers();
    if (!this->pointsUpdated) updatePoints(context);

    glEnableVertexAttribArray(context->program().attrib("point"));
    glVertexAttribPointer(context->program().attrib("point"), 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderableTrajectory::updatePoints(Context *context) {
    const int floatPerVert = 4; // Le dernier pour l'ID du vertex
    const int verticesSize = floatPerVert * points.size();
    float* vertices = new float[verticesSize];

    auto iter = points.begin();
    for (int i = 0 ; i < points.size() ; i++) {
        vertices[i] = i;
        vertices[i + 1] = (*iter).x;
        vertices[i + 2] = (*iter).y;
        vertices[i + 3] = (*iter).z;
        iter++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    delete[] vertices;
    this->pointsUpdated = true;
}