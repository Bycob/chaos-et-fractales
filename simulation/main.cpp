//
// Created by louis on 06/10/16.
//

#include <algorithm>
#include <math.h>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <csignal>
#include <cstring>

#include "utils.h"
#include "Simulation.h"
#include "FileBuffer.h"
#include "rendering/Window.h"
#include "rendering/Context.h"
#include "rendering/Scene.h"
#include "rendering/Renderable.h"
#include "rendering/RenderableSphere.h"
#include "rendering/RenderableTrajectory.h"
#include "physics/World.h"
#include "physics/Body.h"

//TODO rajouter des constantes
#define CLEAR_COLOR_R 3
#define CLEAR_COLOR_G 0
#define CLEAR_COLOR_B 24



//DECLARATIONS

namespace runtime {
    bool running = true;

    /// La fenêtre principale de la simulation
    std::unique_ptr<Window> window;
    /// L'objet Simulation qui contient les planètes
    std::unique_ptr<Simulation> simulation;

    /// Indique, dans la simulation, quelle est la planète sur laquelle est
    /// centrée la vue actuellement. Si vaut -1, alors la vue est centrée sur
    /// 0, 0
    int currentPlanet = 0;

    /** Si true, alors les trajectoires  */
    bool enableTrajectory = true;
}

namespace parameters {
    /// true si les données sont envoyées par la sortie standart pour pouvoir
    /// être exploitées par une autre application.
    bool pipeMode = false;
    /** Si vaut true, alors les résultats de la simulation sont affichés
     * en 3D dans la fenêtre.
     * Si vaut false, les résultats de la simulation sont affichés via
     * la console (dans tous les cas, les résultats sont enregistrés dans
     * un fichier à la fin.*/
    bool enableRender = true;

    int fps = 50;
}




/** Initialise la scène ainsi que le monde physique. */
void createScene();
/** J'ajoute le système solaire à la scène ! */
void addSolarSystem();
void addMooreSystem();




/** Cette méthode est le coeur du thread graphique,
 * Elle crée la fenetre, charge les ressources, effectue
 * la boucle principale de l'application, gère les
 * entrées sorties, puis détruit les ressources lorsque
 * l'utilisateur ferme la fenetre. */
void start();



void input(GLFWwindow *window);
void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

///le séparateur
bool _______IMPLEMENTATIONS__________________________________;

//MAIN

/* Arguments de la ligne de commande :
 * --method [méthode]
 * -m [methode] : choisir la méthode de calcul. Méthodes disponibles :
 * Euler, Runge-Kutta.
 * --input [dossier]
 * -i [dossier] : continuer une simulation précédente. Le dossier
 * indique l'emplacement de la sauvegarde.
 * --system [système]
 * -S [système] : choisir le système de départ (système solaire, ...)
 * --tick [secondes]
 * -t [secondes] choisir la durée d'un tick, qui détermine la vitesse de la simulation
 * -s [secondes] choisir le pas physique de la simulation */

int main(int argc, char** argv) {
    //printf("nombre : %.80Lf\n", M_PIl); //;)
    srand(time(NULL));

    //TODO déterminer la date
    //TODO pouvoir reprendre les résultats de la simulation précédente

    //TODO Analyse des arguments => faire quelque chose de propre
    for (int i = 0 ; i < argc ; i++) {
        if (strcmp(argv[i], "pipe") == 0) {
            parameters::pipeMode = true;
        }
        else if (strcmp(argv[i], "norender") == 0) {
            parameters::enableRender = false;
        }
    }

    createScene();
    start();

    return 0;
}




// CREATION DE LA SCENE


void createScene() {
    runtime::simulation = std::make_unique<Simulation>("files");
    addMooreSystem();
    runtime::simulation->setTrajectoryVisibility(runtime::enableTrajectory);

    runtime::simulation->scene().setSphereMap("assets/galaxy.png");
}

void addSolarSystem() {
    //Constantes
    Light light(LIGHT_POINT, 0, 0, 0);
    runtime::simulation->scene().setLight(light);

    //dist 1e10 m, mass 1e21 kg, tps 1e6 s, vit 1e4 m.s-1 -> G *= 1e3 (- 10*3 + 21 + 6*2)
    runtime::simulation->world().setGravityConstant(GRAVITY_CONSTANT * 1e3);

    //Rendus
    auto moonRender = std::make_shared<RenderableSphere>(0.8, 64, 64);
    moonRender->addTexturePath("assets/moon.png");

    auto earthRender = std::make_shared<RenderableSphere>(0.8, 64, 64);
    earthRender->addTexturePath("assets/earth.png");

    auto jupiterRender = std::make_shared<RenderableSphere>(1.5, 64, 64);
    jupiterRender->addTexturePath("assets/jupiter.png");

    auto marsRender = std::make_shared<RenderableSphere>(0.7, 64, 64);
    marsRender->addTexturePath("assets/mars.png");

    auto sunRender = std::make_shared<RenderableSphere>(3, 64, 64);
    sunRender->getMaterial().setAmbient(0.85f, 0.25f, 0.1f);
    sunRender->addTexturePath("assets/sun.png");
    sunRender->getMaterial().setEmit(true);

    //Physique
    auto moonBody = std::make_shared<Body>(73.0);
    moonBody->setPosition(-20.0, 0, 0);
    moonBody->setSpeed(0, 5e-6, 0);

    auto earthBody = std::make_shared<Body>(6000.0);
    earthBody->setPosition(-15, 0, 0);
    earthBody->setSpeed(0, 2.9, 0);

    auto marsBody = std::make_shared<Body>(600.0);
    marsBody->setPosition(0, -24.9, 0);
    marsBody->setSpeed(2.19, 0, 0);

    auto jupiterBody = std::make_shared<Body>(1.9e6);
    jupiterBody->setPosition(0, 77.8, 0);
    jupiterBody->setSpeed(-1.3, 0, 0);

    auto sunBody = std::make_shared<Body>(2e9);

    //Ajout des objets
    runtime::simulation->addPlanet({"sun", sunBody, sunRender});
    runtime::simulation->addPlanet({"earth", earthBody, earthRender});
    //runtime::simulation->addPlanet({"moon", moonBody, moonRender});
    runtime::simulation->addPlanet({"mars", marsBody, marsRender});
    runtime::simulation->addPlanet({"jupiter", jupiterBody, jupiterRender});
}

void addMooreSystem() {
    //Constantes
    Light light(LIGHT_SUN, 1, 1, 1);
    runtime::simulation->scene().setLight(light);

    runtime::simulation->world().setGravityConstant(GRAVITY_CONSTANT);

    //Rendus
    auto body1Render = std::make_shared<RenderableSphere>(0.8, 64, 64);
    body1Render->getMaterial().setDiffuse(1, 0, 0);

    auto body2Render = std::make_shared<RenderableSphere>(0.8, 64, 64);
    body2Render->getMaterial().setDiffuse(0, 1, 0);

    auto body3Render = std::make_shared<RenderableSphere>(0.8, 64, 64);
    body3Render->getMaterial().setDiffuse(0, 0, 1);

    //Physique
    auto body1body = std::make_shared<Body>(1.501e13);
    body1body->setPosition(0, 0, 0);
    body1body->setSpeed(10.00, 10, 0);

    auto body2body = std::make_shared<Body>(1.5e13);
    body2body->setPosition(10, 0, 0);
    body2body->setSpeed(-5.00, -5, 0);

    auto body3body = std::make_shared<Body>(1.5e13);
    body3body->setPosition(-10, 0, 0);
    body3body->setSpeed(-5, -5, 0);

    //Ajout des objets
    runtime::simulation->addPlanet({"body1", body1body, body1Render});
    runtime::simulation->addPlanet({"body2", body2body, body2Render});
    runtime::simulation->addPlanet({"body3", body3body, body3Render});

    //couleurs des trajectoires (temporaires)
    runtime::simulation->setRVBTrajectory();
}





// BOUCLE PRINCIPALE




void start() {
    runtime::running = true;

    if (parameters::enableRender) {
        runtime::window = std::make_unique<Window>();

        runtime::window->setKeyCallback(glfwKeyCallback);
        runtime::window->setScrollCallback(glfwScrollCallback);

        glClearColor(CLEAR_COLOR_R / 255.0f, CLEAR_COLOR_G / 255.0f, CLEAR_COLOR_B / 255.0f, 1);
    }
    else if (!parameters::pipeMode) {
        auto sigstop = [] (int signum) {
            std::cout << "Simulation interrompue. Ecriture des données..." << std::endl;
            runtime::running = false;
        };

        std::signal(SIGINT, sigstop);
        std::signal(SIGTERM, sigstop);

        std::cout << "Démarrage de la simulation. Ctrl-C pour interrompre la simulation." << std::endl;
    }

    while (runtime::running) {
        auto before = std::chrono::system_clock::now();

        runtime::simulation->update(1.0 / parameters::fps, parameters::pipeMode);

        if (parameters::enableRender) {

            //Rendu
            runtime::window->setupFrame();

            input(runtime::window->window());
            runtime::simulation->scene().render(runtime::window->context());

            runtime::window->finalizeFrame();

            //Mise à jour du flag
            if (runtime::window->shouldClose()) {
                runtime::running = false;
            }

            //Dormir
            auto after = std::chrono::system_clock::now();
            float elapsed = std::chrono::duration_cast<std::chrono::microseconds>(after - before).count() / 1000000.0f;

            sleep(std::max(1.0f / parameters::fps - elapsed, 0.0f));
            //std::cout << elapsed << std::endl; // -> profiling
        }
        else {

            // Pour laisser le temps à python de respirer
            if (parameters::pipeMode) {
                sleep(30);
            }
        }
    }

    //Ecriture des fichiers contenant les données.
    runtime::simulation->writeFiles();

    if (parameters::enableRender) {
        runtime::window->destroy();
    }
    else if (!parameters::pipeMode) {
        std::cout << "Données écrites" << std::endl;
    }
}




//ENTREES SORTIES




void input(GLFWwindow * window) {

    /*
     | -> LEFT et RIGHT pour tourner autour de l'axe UP (quasi toujours vers le haut)
     | -> UP et DOWN pour faire pivoter la position de la cam vers le haut ou le bas (limite aux extremes)
     | -> 0 pour la vue de dessus centrée sur 0, 0
     + -> En vue de dessus, LEFT et RIGHT font tourner l'axe UP selon l'axe Z.
     | -> vue de côté pour chacun des astres sur les touches 1, 2, 3, 4 ...
     | -> Rotation molette pour le zoom.
      */
    //Controles
    int rightKey = glfwGetKey(window, GLFW_KEY_RIGHT);
    int leftKey = glfwGetKey(window, GLFW_KEY_LEFT);
    int upKey = glfwGetKey(window, GLFW_KEY_UP);
    int downKey = glfwGetKey(window, GLFW_KEY_DOWN);

    Scene & scene = runtime::simulation->scene();

    if (!scene.camera().isTraveling()) {
        //Mise à jour en fonction de la planete actuelle
        if (runtime::currentPlanet != -1 && runtime::currentPlanet < runtime::simulation->getPlanetCount()) {
            glm::vec3 planetGraphicalPos = runtime::simulation->getPlanet(runtime::currentPlanet).render->getPosition();
            scene.camera().moveCameraByCenterPoint(planetGraphicalPos.x, planetGraphicalPos.y, planetGraphicalPos.z);
        }

        glm::vec3 cameraUp = scene.camera().getUp();

        if (cameraUp.x == 0 && cameraUp.y == 0) {
            if (rightKey == GLFW_PRESS && leftKey != GLFW_PRESS) {
                scene.camera().rotateZ(180.0f / 70.0f);
            }
            else if (leftKey == GLFW_PRESS && rightKey != GLFW_PRESS) {
                scene.camera().rotateZ(- 180.0f / 70.0f);
            }

            if (upKey == GLFW_PRESS && downKey != GLFW_PRESS) {
                scene.camera().rotateUpDown(180.0f / 70.0f);
            }
            else if (downKey == GLFW_PRESS && upKey != GLFW_PRESS) {
                scene.camera().rotateUpDown(- 180.0f / 70.0f);
            }
        }
        else if (cameraUp.z == 0){
            //TODO contrôle de la caméra en vue de dessus
        }
    }
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_KP_0) {
            runtime::currentPlanet = -1;

            auto traveling = std::make_unique<Traveling>(
                    runtime::simulation->scene().camera(), 0, 0, 30, 0, 0, 0, 0, 1, 0
            );
            traveling->setDuration(0.5f);
            runtime::simulation->scene().camera().traveling(traveling);
        }
        else if (key >= GLFW_KEY_KP_1 && key <= GLFW_KEY_KP_9){
            int targetPlanet = key - GLFW_KEY_KP_1;

            if (targetPlanet < runtime::simulation->getPlanetCount()) {
                runtime::currentPlanet = targetPlanet;
                runtime::simulation->setCameraPlanet(runtime::currentPlanet);
            }
        }
        else if (key == GLFW_KEY_T) {
            runtime::enableTrajectory = ! runtime::enableTrajectory;
            runtime::simulation->setTrajectoryVisibility(runtime::enableTrajectory);
        }
    }
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    runtime::simulation->scene().camera().zoom((float) pow(1.2, - yoffset));
}
