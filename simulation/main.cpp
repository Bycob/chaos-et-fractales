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

#define CLEAR_COLOR_R 3
#define CLEAR_COLOR_G 0
#define CLEAR_COLOR_B 24

#define COMMAND_COUNT 4
#define HELP_COMMAND_SIZE 40

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

    /** Si true, alors les trajectoires sont visibles */
    bool enableTrajectory = true;
    /// Si true alors on voit les planètes.
    bool displayPlanets = true;
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

    std::vector<std::string> filenames;

    int fps = 50;
}



/** Recrée la scène actuelle. */
void recreateScene();
/** Initialise la scène ainsi que le monde physique.*/
void createScene();
/// Le monde par défaut
void addMooreSystem();



void printHelp();
/** Cette méthode est le coeur du thread graphique,
 * Elle crée la fenetre, charge les ressources, effectue
 * la boucle principale de l'application, gère les
 * entrées sorties, puis détruit les ressources lorsque
 * l'utilisateur ferme la fenetre. */
void start();



void input(GLFWwindow *window);
void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void printKeys();

///le séparateur
bool _______IMPLEMENTATIONS__________________________________;

//MAIN

void printHelp() {

    std::string commands[COMMAND_COUNT][2] = {
            {"--help, -h", "Affiche ce message"},
            {"--norender", "Lance uniquement la simulation physique, sans l'affichage"},
            {"--pipe, -p", "Affiche en temps réel les données calculées pendant la simulation. "
                    "Cette option est adaptée à une utilisation en pipe avec l'utilitaire display.py."},
            {"--files, -f file1[;file2[;...]]", "Indique les fichiers de description à partir desquels "
                "la simulation sera construite."}
    };

    std::cout << "\nBienvenue dans l'aide du programme intitulé très sobrement \"Simulation\"." << std::endl << std::endl
            << "Usage : \n\tsimulation [arguments...]" << std::endl << std::endl
            << "Commandes disponibles : " << std::endl;

    printCommandDict(commands, COMMAND_COUNT, HELP_COMMAND_SIZE);
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    //printf("nombre : %.80f\n", sqrt(10.0)); //;)
    srand(time(NULL));

    //TODO déterminer la date
    //TODO pouvoir reprendre les résultats de la simulation précédente

    for (int i = 0 ; i < argc ; i++) {
        //Traitement générique
        std::string arg(argv[i]);
        std::string param;
        if (argc > i + 1 && argv[i + 1][0] != '-') {
            param = std::string(argv[i + 1]);
        }

        if (arg == "--pipe" || arg == "-p") {
            parameters::pipeMode = true;
        }
        else if (arg == "--norender") {
            parameters::enableRender = false;
        }
        else if (arg == "-f" || arg == "--files") {
            if (param != "") {
                parameters::filenames.push_back(param);
            }
            //TODO split
        }
        else if (arg == "--help" || arg == "-h") {
            printHelp();
            exit(0);
        }
    }

    createScene();
    start();

    return 0;
}




// CREATION DE LA SCENE

void recreateScene() {
    //On effectue les derniers traitements
    runtime::currentPlanet = 0;

    //On raffraichit la scène.
    createScene();
}

void createScene() {
    // On essaie avec les noms de fichiers
    bool fileParsed = false;
    if (parameters::filenames.size() != 0) {
        try {
            runtime::simulation = std::make_unique<Simulation>("files", parameters::filenames[0]);
            fileParsed = true;
        }
        catch (std::runtime_error & e) {
            std::cerr << e.what() << std::endl;
        }
    }

    //Si on peut pas on met le système de Moore par défaut.
    if (!fileParsed) {
        runtime::simulation = std::make_unique<Simulation>("files");
        addMooreSystem();
    }

    //Réglages génériques
    runtime::simulation->setTrajectoryVisibility(runtime::enableTrajectory);
    runtime::simulation->setPlanetVisibility(runtime::displayPlanets);

    runtime::simulation->scene().setSphereMap("assets/galaxy.png");
}

void addMooreSystem() {
    //Constantes
    Light light(LIGHT_SUN, 1, 1, 1);
    runtime::simulation->scene().setLight(light);

    runtime::simulation->world().setGravityConstant(GRAVITY_CONSTANT);
    runtime::simulation->world().setMethod(Method::RUNGE_KUTTA);

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
    runtime::simulation->set3BodiesSpecial();
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
        // Vue de dessus
        if (key == GLFW_KEY_KP_0) {
            runtime::currentPlanet = -1;

            auto traveling = std::make_unique<Traveling>(
                    runtime::simulation->scene().camera(), 0, 0, 30, 0, 0, 0, 0, 1, 0
            );
            traveling->setDuration(0.5f);
            runtime::simulation->scene().camera().traveling(traveling);
        }
        // Vue planètes
        else if (key >= GLFW_KEY_KP_1 && key <= GLFW_KEY_KP_9){
            int targetPlanet = key - GLFW_KEY_KP_1;

            if (targetPlanet < runtime::simulation->getPlanetCount()) {
                runtime::currentPlanet = targetPlanet;
                runtime::simulation->setCameraPlanet(runtime::currentPlanet);
            }
        }
        // Toggle trajectoires
        else if (key == GLFW_KEY_T && mods == 0) {
            runtime::enableTrajectory = ! runtime::enableTrajectory;
            runtime::simulation->setTrajectoryVisibility(runtime::enableTrajectory);
        }
        // Toggle planets
        else if (key == GLFW_KEY_P) {
            runtime::displayPlanets = ! runtime::displayPlanets;
            runtime::simulation->setPlanetVisibility(runtime::displayPlanets);
        }
        // Reset trajectory
        else if (key == GLFW_KEY_T && (mods & GLFW_MOD_CONTROL) != 0) {
            runtime::simulation->resetTrajectories();
        }
        // Pause / Lancement
        else if (key == GLFW_KEY_SPACE) {
            runtime::simulation->togglePaused();
        }
        // Sens d'avance de la simulation
        else if (key == GLFW_KEY_F) {
            runtime::simulation->setReverse(false);
        }
        else if (key == GLFW_KEY_B) {
            runtime::simulation->setReverse(true);
        }
        //contrôleur de temps
        else if (key == GLFW_KEY_KP_ADD) {
            runtime::simulation->incrementTimeMultiplier();
        }
        else if (key == GLFW_KEY_KP_SUBTRACT) {
            runtime::simulation->decrementTimeMultiplier();
        }
        // Reload file
        else if (key == GLFW_KEY_ENTER && (mods & (GLFW_MOD_CONTROL + GLFW_MOD_SHIFT)) != 0) {
            recreateScene();
        }
        // Quit (Ctrl+Q)
        else if (key == GLFW_KEY_A && (mods & (GLFW_MOD_CONTROL)) != 0) {//A us = Q fr
            runtime::running = false;
        }
    }
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    runtime::simulation->scene().camera().zoom((float) pow(1.2, - yoffset));
}
