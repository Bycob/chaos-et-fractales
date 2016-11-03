//
// Created by louis on 07/10/16.
//

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include "Context.h"
#include "Program.h"

//Traveling
Traveling::Traveling(Camera &camera, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) :
        eyeX(eyeX), eyeY(eyeY), eyeZ(eyeZ), centerX(centerX), centerY(centerY), centerZ(centerZ), upX(upX), upY(upY), upZ(upZ) {
    
    timeStarted = std::chrono::system_clock::now();
    
    auto startEye = camera.getEye();
    auto startCenter = camera.getCenter();
    auto startUp = camera.getUp(); //EXCELLENT !
    
    startEyeX = startEye.x;
    startEyeY = startEye.y;
    startEyeZ = startEye.z;
    
    startCenterX = startCenter.x;
    startCenterY = startCenter.y;
    startCenterZ = startCenter.z;
    
    startUpX = startUp.x;
    startUpY = startUp.y;
    startUpZ = startUp.z;
}

void Traveling::setDuration(float duration) {
    this->duration = duration;
}

void Traveling::applyImmediatly(Camera &camera) {
    camera.setEye(eyeX, eyeY, eyeZ);
    camera.setCenter(centerX, centerY, centerZ);
    camera.setUp(upX, upY, upZ);
}

void Traveling::update(Camera &camera) {
    auto now = std::chrono::system_clock::now();
    auto timeElapsed = now - this->timeStarted;

    float ratio = std::chrono::duration_cast<std::chrono::microseconds>(now - this->timeStarted).count()
                  / (float) 1000000.0 / this->duration;

    if (ratio >= 1) this->finished = true;
    else {
        camera.setEye(
                startEyeX + (eyeX - startEyeX) * ratio,
                startEyeY + (eyeY - startEyeY) * ratio,
                startEyeZ + (eyeZ - startEyeZ) * ratio
        );
        
        camera.setCenter(
                startCenterX + (centerX - startCenterX) * ratio,
                startCenterY + (centerY - startCenterY) * ratio,
                startCenterZ + (centerZ - startCenterZ) * ratio
        );

        camera.setUp(
                startUpX + (upX - startUpX) * ratio,
                startUpY + (upY - startUpY) * ratio,
                startUpZ + (upZ - startUpZ) * ratio
        );
    }
}



void Camera::setEye(float eyeX, float eyeY, float eyeZ) {
    this->eyeX = eyeX;
    this->eyeY = eyeY;
    this->eyeZ = eyeZ;
}

void Camera::setCenter(float centerX, float centerY, float centerZ) {
    this->centerX = centerX;
    this->centerY = centerY;
    this->centerZ = centerZ;
}

void Camera::setUp(float upX, float upY, float upZ) {
    this->upX = upX;
    this->upY = upY;
    this->upZ = upZ;
}

glm::vec3 Camera::getEye() {
    return glm::vec3(eyeX, eyeY, eyeZ);
}

glm::vec3 Camera::getCenter() {
    return glm::vec3(centerX, centerY, centerZ);
}

glm::vec3 Camera::getUp() {
    return glm::vec3(upX, upY, upZ);
}

void Camera::rotateZ(float degrees) {
    const double radians = (float) (degrees * (M_PI / 180.0));

    double diffX = this->eyeX - this->centerX;
    double diffY = this->eyeY - this->centerY;

    this->eyeX = (float) (diffX * cos(radians) - diffY * sin(radians) + this->centerX);
    this->eyeY = (float) (diffX * sin(radians) + diffY * cos(radians) + this->centerY);
}

void Camera::travelEye(float eyeX, float eyeY, float eyeZ, float seconds) {
    auto traveling = std::make_unique<Traveling>(*this, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    traveling->setDuration(seconds);
    this->traveling(traveling);
}

void Camera::traveling(std::unique_ptr<Traveling> &traveling) {
    this->_traveling = std::move(traveling);
}

void Camera::updateCamera() {
    if (this->_traveling == nullptr) return;

    if (this->_traveling->isFinished()) {
        this->_traveling->applyImmediatly(*this);
        this->_traveling = nullptr;
    }
    else {
        this->_traveling->update(*this);
    }
}

void Camera::setCameraView(Context *context) {
    //Frustum
    glm::mat4 projection = glm::perspective(glm::radians(fov), ratio, zNear, zFar);

    //Look at
    glm::mat4 camera = glm::lookAt(glm::vec3(eyeX, eyeY, eyeZ), glm::vec3(centerX, centerY, centerZ), glm::vec3(upX, upY, upZ));

    if (context != nullptr) {
        context->program().setUniform3f("cameraPos", eyeX, eyeY, eyeZ);
        context->program().setUniformMatrix4("projection", projection);
        context->program().setUniformMatrix4("camera", camera);
    }
}