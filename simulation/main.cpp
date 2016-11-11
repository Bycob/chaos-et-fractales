//
// Created by louis on 06/10/16.
//

#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <zconf.h>
#include <memory>
#include <csignal>

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

struct Planet {
    Planet(std::string name, std::shared_ptr<Body> body, std::shared_ptr<RenderableSphere> render);
    std::string name;
    std::shared_ptr<Body> body;
    std::shared_ptr<RenderableSphere> render;
    std::shared_ptr<RenderableTrajectory> trajectory;
    FileBuffer buffer;
};

namespace runtime {
    std::string DATE;

    /// La fenêtre principale de la simulation
    std::unique_ptr<Window> window;
    /// La scène qui contient les objets à afficher (planètes, trajectoires...)
    std::unique_ptr<Scene> scene = nullptr;
    /// Le monde qui contient les parties physiques des objets
    std::unique_ptr<World> world = nullptr;
    /// L'ensemble des planètes de la simulation.
    std::vector<Planet> planets;

    /// Indique, dans la simulation, quelle est la planète sur laquelle est
    /// centrée la vue actuellement. Si vaut -1, alors la vue est centrée sur
    /// 0, 0
    int currentPlanet = 0;

    /** Si true, alors les trajectoires  */
    bool enableTrajectory = true;
    /// Indique la planète qui est au centre du système. Si == -1,
    /// alors le centre du système se trouve en 0, 0
    int originObject = 0;

    /// L'échelle de temps, càd combien de temps passe dans la simulation (en 1e6 s)
    /// lorsqu'il s'écoule 1 seconde.
    double timeScale = 1;
    /// Le pas de discrétisation physique de la simulation.
    double physicalStep = 0.004;
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


void addPlanet(std::string name, double mass, double x, double y, double z, double size);
/** Ajoute une planète à la scene et au monde physique. */
void addPlanet(Planet planet);




/** Initialise la scène ainsi que le monde physique. */
void createScene();
/** J'ajoute le système solaire à la scène ! */
void addSolarSystem();





void updateTrajectoryVisibility();



/** Cette méthode est le coeur du thread graphique,
 * Elle crée la fenetre, charge les ressources, effectue
 * la boucle principale de l'application, gère les
 * entrées sorties, puis détruit les ressources lorsque
 * l'utilisateur ferme la fenetre. */
void start();

/** Cette méthode est appelée à chaque tour de boucle pour
 * analyser les entrées sorties, mettre à jour le monde physique
 * et dessiner la scene à l'écran. */
void stepSimulation();

/** Ecrit les résultats de la simulation dans les fichiers prévus
 * à cet effet. */
void writeFiles();



void input(GLFWwindow *window);
void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);


//MAIN

/* Arguments de la ligne de commande :
 * --method [méthode]
 * -m [methode] : choisir la méthode de calcul. Méthodes disponibles :
 * Euler, Runge-Kotta.
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

    //TODO déterminer la date
    //TODO pouvoir reprendre les résultats de la simulation précédente

    //TODO Analyse des arguments

    createScene();
    start();

    writeFiles();

    return 0;
}

void writeFiles() {
    for (Planet &object : runtime::planets) {
        object.buffer.writeData();
    }
}

void updateTrajectoryVisibility() {
    for (Planet &planet : runtime::planets) {
        if (planet.trajectory != nullptr) {
            planet.trajectory->setActive(runtime::enableTrajectory);
        }
    }
}




// CREATION DE LA SCENE


void createScene() {
    runtime::scene = std::make_unique<Scene>();

    Light light(LIGHT_POINT, 0, 0, 0);
    runtime::scene->setLight(light);

    //dist 1e10 m, mass 1e21 kg, tps 1e6 s, vit 1e4 m.s-1 -> G *= 1e3 (- 10*3 + 21 + 6*2)
    runtime::world = std::make_unique<World>();
    runtime::world->setGravityConstant(GRAVITY_CONSTANT * 1e3);

    addSolarSystem();

    updateTrajectoryVisibility();
}

void addSolarSystem() {
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

    auto sunBody = std::make_shared<Body>(1e9);

    //Ajout des objets
    addPlanet({"sun", sunBody, sunRender});
    addPlanet({"earth", earthBody, earthRender});
    //addPlanet({"moon", moonBody, moonRender});
    addPlanet({"mars", marsBody, marsRender});
    addPlanet({"jupiter", jupiterBody, jupiterRender});
}


// -----

Planet::Planet(std::string name, std::shared_ptr<Body> body, std::shared_ptr<RenderableSphere> render)
        : name(name), body(body), render(render), buffer(name + "_" + runtime::DATE),
          trajectory(std::make_shared<RenderableTrajectory>()) {

}


void addPlanet(std::string name, double mass, double x, double y, double z, double size) {
    auto sphereRender = std::make_shared<RenderableSphere>(size, 64, 64);
    sphereRender->getMaterial().setDiffuse(0.85f, 0.75f, 0.1f);
    auto sphereBody = std::make_shared<Body>(mass);
    sphereBody->setPosition(x, y, z);
    addPlanet({name, sphereBody, sphereRender});
}

void addPlanet(Planet planet) {
    runtime::planets.push_back(planet);
    if (planet.body != nullptr) {
        runtime::world->addObject(planet.body);
    }
    if (planet.render != nullptr) {
        runtime::scene->addObject(planet.render);
        runtime::scene->addObject(planet.trajectory);
        planet.render->getMaterial().setSpecular(0, 0, 0);
    }
}




// BOUCLE PRINCIPALE




void start() {
    bool running = true;

    if (parameters::enableRender) {
        runtime::window = std::make_unique<Window>();

        runtime::window->setKeyCallback(glfwKeyCallback);
        runtime::window->setScrollCallback(glfwScrollCallback);

        glClearColor(CLEAR_COLOR_R / 255.0f, CLEAR_COLOR_G / 255.0f, CLEAR_COLOR_B / 255.0f, 1);
    }
    else {
        auto sigstop = [](int signum) {
            std::cout << "Simulation interrompue. Ecriture des données..." << std::endl;
            writeFiles();
            std::cout << "Données écrites" << std::endl;

            std::exit(0);
        };

        std::signal(SIGINT, sigstop);
        std::signal(SIGTERM, sigstop);

        std::cout << "Démarrage de la simulation. Ctrl-C pour interrompre la simulation." << std::endl;
    }

    while (running) {
        stepSimulation();

        if (parameters::enableRender) {
            runtime::window->setupFrame();
            input(runtime::window->window());
            runtime::scene->render(runtime::window->context());

            runtime::window->finalizeFrame();
            usleep(20000); //TODO stabiliser le fps

            if (runtime::window->shouldClose()) {
                running = false;
            }
        }
        else {

        }
    }

    if (parameters::enableRender) {
        runtime::window->destroy();
    }
}




void stepSimulation() {

    //Mise à jour du monde (précision ~ 1h) t*10e6
    const double timeScale = runtime::timeScale / parameters::fps;
    const double baseStep = runtime::physicalStep;
    runtime::world->step(timeScale, (int) (timeScale / baseStep));

    for (Planet &planet : runtime::planets) {
        auto &body = planet.body;

        if (body != nullptr) {
            double x = body->getX();
            double y = body->getY();
            double z = body->getZ();

            if (runtime::originObject != -1 && runtime::planets.size() > runtime::originObject) {
                x -= runtime::planets[runtime::originObject].body->getX();
                y -= runtime::planets[runtime::originObject].body->getY();
                z -= runtime::planets[runtime::originObject].body->getZ();
            }

            //Mise à jour du rendu de l'objet
            if (parameters::enableRender) {
                planet.render->setPosition((float) x, (float) y, (float) z);
                planet.trajectory->addPoint((float) x, (float) y, (float) z);
            }

            //Envoi des coordonnées dans le buffer
            std::string separator = ";";
            std::ostringstream stringstream;
            stringstream << runtime::world->getTime() << separator << x << separator << y << separator << z;
            planet.buffer.addLine(stringstream.str());

            if (parameters::pipeMode) {
                //Envoie des coordonnées sur la sortie stdout
                std::cout << planet.name << ":" << stringstream.str() << std::endl;
            }
        }
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

    if (!runtime::scene->camera().isTraveling()) {
        //Mise à jour en fonction de la planete actuelle
        if (runtime::currentPlanet != -1 && runtime::currentPlanet < runtime::planets.size()) {
            glm::vec3 planetGraphicalPos = runtime::planets[runtime::currentPlanet].render->getPosition();
            runtime::scene->camera().moveCameraByCenterPoint(planetGraphicalPos.x, planetGraphicalPos.y, planetGraphicalPos.z);
        }

        glm::vec3 cameraUp = runtime::scene->camera().getUp();

        if (cameraUp.x == 0 && cameraUp.y == 0) {
            if (rightKey == GLFW_PRESS && leftKey != GLFW_PRESS) {
                runtime::scene->camera().rotateZ(180.0f / 70.0f);
            }
            else if (leftKey == GLFW_PRESS && rightKey != GLFW_PRESS) {
                runtime::scene->camera().rotateZ(- 180.0f / 70.0f);
            }

            if (upKey == GLFW_PRESS && downKey != GLFW_PRESS) {
                runtime::scene->camera().rotateUpDown(180.0f / 70.0f);
            }
            else if (downKey == GLFW_PRESS && upKey != GLFW_PRESS) {
                runtime::scene->camera().rotateUpDown(- 180.0f / 70.0f);
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
                    runtime::scene->camera(), 0, 0, 30, 0, 0, 0, 0, 1, 0
            );
            traveling->setDuration(0.5f);
            runtime::scene->camera().traveling(traveling);
        }
        else if (key >= GLFW_KEY_KP_1 && key <= GLFW_KEY_KP_9){
            int targetPlanet = key - GLFW_KEY_KP_1;

            if (targetPlanet < runtime::planets.size()) {
                runtime::currentPlanet = targetPlanet;

                //Utilisation de la position graphique
                glm::vec3 planetPos = runtime::planets[runtime::currentPlanet].render->getPosition();

                //TODO le 15 en fonction du radius de la planète.

                auto traveling = std::make_unique<Traveling>(
                        runtime::scene->camera(),
                        planetPos.x + 15, planetPos.y + 15, planetPos.z + 15,
                        planetPos.x, planetPos.y, planetPos.z, 0, 0, 1
                );
                traveling->setDuration(0.5f);
                runtime::scene->camera().traveling(traveling);
            }
        }
        else if (key == GLFW_KEY_T) {
            runtime::enableTrajectory = ! runtime::enableTrajectory;
            updateTrajectoryVisibility();
        }
    }
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    runtime::scene->camera().zoom((float) pow(1.2, - yoffset));
}
