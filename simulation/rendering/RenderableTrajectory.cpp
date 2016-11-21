//
// Created by louis on 06/11/16.
//

#include <glm/glm.hpp>
#include <iostream>

#include "RenderableTrajectory.h"
#include "Context.h"
#include "Scene.h"

RenderableTrajectory::RenderableTrajectory(int maxPointCount) : maxPointCount(maxPointCount) {
    _usesBlendMode = true;
}

void RenderableTrajectory::setColor(float r, float g, float b) {
    this->color = glm::vec3(r, g, b);
}

void RenderableTrajectory::addPoint(float x, float y, float z) {
    points.push_back(glm::vec3(x, y, z));

    while (points.size() > maxPointCount) {
        points.pop_front();
    }

    this->pointsUpdated = false;
}

bool RenderableTrajectory::shouldRender() {
    return Renderable::shouldRender() && this->points.size() != 0;
}

void RenderableTrajectory::render(Context *context, Scene *scene) {
    if (!this->shouldRender()) return;

    if (!this->compiled) buildRenderData(context);
    if (!this->pointsUpdated) updatePoints(context);

    context->setCurrentProgram("trajectory");

    context->program().setUniform3f("color", color.r, color.g, color.b);
    context->program().setUniform1i("count", points.size());

    glDepthMask(GL_FALSE);

    glBindVertexArray(this->gVAO);
    glDrawArrays(GL_LINE_STRIP, 0, this->points.size());
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}

void RenderableTrajectory::regenerateBuffers() {
    Renderable::regenerateBuffers();

    glGenBuffers(1, &this->tangentBuffer);
}

void RenderableTrajectory::deleteBuffers() {
    Renderable::deleteBuffers();

    if (this->tangentBuffer != 0) {
        glDeleteBuffers(1, &this->tangentBuffer);
        this->tangentBuffer = 0;
    }
}

void RenderableTrajectory::buildRenderData(Context *context) {
    context->setCurrentProgram("trajectory");

    regenerateBuffers();
    if (!this->pointsUpdated) updatePoints(context);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(context->program().attrib("point"));
    glVertexAttribPointer(context->program().attrib("point"), 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glEnableVertexAttribArray(context->program().attrib("tangent"));
    glVertexAttribPointer(context->program().attrib("tangent"), 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->compiled = true;
}

void RenderableTrajectory::updatePoints(Context *context) {
    const int floatPerVert = 4; // Le dernier pour l'ID du vertex
    const int floatPerTangent = 3;
    const int verticesSize = floatPerVert * points.size();
    const int tangentSize = floatPerTangent * points.size();
    float* vertices = new float[verticesSize];
    float* tangents = new float[tangentSize];

    auto iter = points.begin();
    for (int i = 0 ; i < points.size() ; i++) {
        vertices[i * floatPerVert] = i;
        vertices[i * floatPerVert + 1] = (*iter).x;
        vertices[i * floatPerVert + 2] = (*iter).y;
        vertices[i * floatPerVert + 3] = (*iter).z;

        iter++;

        if (i != points.size() - 1) {
            tangents[i * floatPerTangent] = (*iter).x - vertices[i * floatPerVert + 1];
            tangents[i * floatPerTangent + 1] = (*iter).y - vertices[i * floatPerVert + 2];
            tangents[i * floatPerTangent + 2] = (*iter).z - vertices[i * floatPerVert + 3];
        }
        else if (i != 0) {
            tangents[i * floatPerTangent] = tangents[(i - 1) * floatPerTangent];
            tangents[i * floatPerTangent + 1] = tangents[(i - 1) * floatPerTangent + 1];
            tangents[i * floatPerTangent + 2] = tangents[(i - 1) * floatPerTangent + 2];
        }
        else {
            tangents[i * floatPerTangent] = 0;
            tangents[i * floatPerTangent + 1] = 0;
            tangents[i * floatPerTangent + 2] = 0;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, tangentSize * sizeof(float), tangents, GL_DYNAMIC_DRAW);

    delete[] vertices;
    delete[] tangents;

    this->pointsUpdated = true;
}