//
// Created by louis on 07/10/16.
//

#ifndef RENDUOPENGL_CAMERA_H
#define RENDUOPENGL_CAMERA_H

#include <glm/glm.hpp>
#include <memory>
#include <ctime>
#include <chrono>

class Scene;
class Context;

class Traveling;

/** Définit la caméra du rendu. */
class Camera {

public :
    Camera(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) :
            eyeX(eyeX), eyeY(eyeY), eyeZ(eyeZ), centerX(centerX), centerY(centerY), centerZ(centerZ), upX(upX), upY(upY), upZ(upZ) { }

    Camera(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ) :
            Camera(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 0, 1) { }

    Camera() : Camera(4, 4, 4, 0, 0, 0, 0, 0, 1) { }

    void setFOV(float fov) {
        this->fov = fov;
    }

    void setRatio(float ratio) {
        this->ratio = ratio;
    }

    void setEye(float eyeX, float eyeY, float eyeZ);
    void setCenter(float centerX, float centerY, float centerZ);
    void setUp(float upX, float upY, float upZ);

    glm::vec3 getEye();
    glm::vec3 getCenter();
    glm::vec3 getUp();

    void rotateZ(float degrees);

    /** Déplace la caméra de sorte que son origine soit à la
     * position indiquée. Le déplacement dure le nombre de secondes
     * passé en paramètres. */
    void travelEye(float eyeX, float eyeY, float eyeZ, float seconds);

    /** Indique si la caméra est actuellement en cours de
     * déplacement. */
    bool isTraveling() {return this->_traveling != nullptr;};

    /** Définit le traveling passé en paramètres comme traveling
     * actuel de la caméra. Le traveling fait ensuite décrire
     * une trajectoire lisse à la caméra, jusqu'au point demandé.*/
    void traveling(std::unique_ptr<Traveling> & traveling);

    void updateCamera();
    void setCameraView(Context *context);

private:

    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;

    float fov = 70;
    float ratio = 1;
    float zNear = 0.1, zFar = 100;

    std::unique_ptr<Traveling> _traveling = nullptr;
};


/** La classe Traveling permet de déplacer la caméra de
 * manière lisse jusqu'à un point désiré. */
class Traveling {

public :
    Traveling(Camera &camera, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);

    Traveling(Camera &camera, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ) :
            Traveling(camera, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 0, 1) { }

    /** Définit la durée du traveling, en secondes. */
    void setDuration(float duration);
    bool isFinished() {return this->finished;};

    /** Méthode généralement appellée à la fin du traveling pour
     * s'assurer que la camera est bien à sa position finale.*/
    void applyImmediatly(Camera &camera);

    /** Cette méthode met à jour la position de la caméra, selon
     * ce traveling.*/
    void update(Camera &camera);

private :
    //Valeurs au début du traveling
    float startEyeX, startEyeY, startEyeZ;
    float startCenterX, startCenterY, startCenterZ;
    float startUpX, startUpY, startUpZ;

    //Valeurs à atteindre
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;

    ///L'instant où le traveling débute en clicks.
    std::chrono::time_point<std::chrono::system_clock> timeStarted;
    ///Durée du traveling de la caméra en secondes.
    float duration = 0.5;
    bool finished = false;
};


#endif //RENDUOPENGL_CAMERA_H
