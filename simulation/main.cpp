//
// Created by louis on 06/10/16.
//

#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <zconf.h>
#include <memory>

#include "FileBuffer.h"
#include "rendering/Context.h"
#include "rendering/Scene.h"
#include "rendering/Renderable.h"
#include "rendering/RenderableSphere.h"
#include "physics/World.h"
#include "physics/Body.h"
#include "rendering/RenderableTrajectory.h"

//DECLARATIONS

//#define SIMULATION_TO_COUT

//TODO rajouter des constantes

struct Planet {
    Planet(std::string name, std::shared_ptr<Body> body, std::shared_ptr<RenderableSphere> render);
    std::string name;
    std::shared_ptr<Body> body;
    std::shared_ptr<RenderableSphere> render;
    std::shared_ptr<RenderableTrajectory> trajectory;
    FileBuffer buffer;
};

std::string DATE;
int currentPlanet = 0;
std::unique_ptr<Scene> scene = nullptr;
std::unique_ptr<World> world = nullptr;
std::vector<Planet> planets;

//TODO struct parameters
bool enableGraphicalSimulation = true;

void addPlanet(std::string name, double mass, double x, double y, double z, double size);

/** Ajoute un objet 3D à la scene et au monde physique. */
void addPlanet(Planet planet);

/** Initialise la scène ainsi que le monde physique. */
void createScene();

/** J'ajoute le système solaire à la scène ! */
void addSolarSystem();


/** Cette méthode est le coeur du thread graphique,
 * Elle crée la fenetre, charge les ressources, effectue
 * la boucle principale de l'application, gère les
 * entrées sorties, puis détruit les ressources lorsque
 * l'utilisateur ferme la fenetre. */
void graphicThread();

/** Cette méthode est appelée à chaque tour de boucle pour
 * analyser les entrées sorties, mettre à jour le monde physique
 * et dessiner la scene à l'écran. */
void mainLoop(GLFWwindow * window, Context *context);

void glfwErrorCallback(int error, const char* description);
void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);


//MAIN

int main(int argc, char** argv) {
    //printf("nombre : %.80Lf\n", M_PIl); //;)

    //TODO déterminer la date
    //TODO pouvoir reprendre les résultats de la simulation précédente

    createScene();

    graphicThread();

    for (Planet &object : planets) {
        object.buffer.writeData();
    }

    return 0;
}


//IMPLEMENTATIONS

Planet::Planet(std::string name, std::shared_ptr<Body> body, std::shared_ptr<RenderableSphere> render)
    : name(name), body(body), render(render), buffer(name + "_" + DATE),
      trajectory(std::make_shared<RenderableTrajectory>()) {


}


void createScene() {
    scene = std::make_unique<Scene>();

    Light light(LIGHT_POINT, 0, 0, 0);
    scene->setLight(light);

    //dist 1e10 m, mass 1e21 kg, tps 1e6 s, vit 1e4 m.s-1 -> G *= 1e3 (- 10*3 + 21 + 6*2)
    world = std::make_unique<World>();
    world->setGravityConstant(6.7e-11 * 1e3);

    addSolarSystem();
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

    auto marsBody = std::make_shared<Body>(6000.0);
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

void addPlanet(std::string name, double mass, double x, double y, double z, double size) {
    auto sphereRender = std::make_shared<RenderableSphere>(size, 64, 64);
    sphereRender->getMaterial().setDiffuse(0.85f, 0.75f, 0.1f);
    auto sphereBody = std::make_shared<Body>(mass);
    sphereBody->setPosition(x, y, z);
    addPlanet({name, sphereBody, sphereRender});
}

void addPlanet(Planet planet) {
    planets.push_back(planet);
    if (planet.body != nullptr) world->addObject(planet.body);
    if (planet.render != nullptr) {
        scene->addObject(planet.render);
        planet.render->getMaterial().setSpecular(0, 0, 0);
    }
}


void graphicThread() {
    int width, height;

    if (!glfwInit()) {
        std::cerr << "Erreur au lancement de glfw." << std::endl;
        return;
    }

    glfwSetErrorCallback(glfwErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_SAMPLES, 16);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simulation", NULL, NULL);

    if (!window) {
        std::cerr << "Erreur au lancement de glfw." << std::endl;
        return;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);

    //Création du contexte
    Context context(window);

    glEnable(GL_MULTISAMPLE);
    glClearColor(3 / 255.0f, 0 / 255.0f, 32 / 255.0f, 1);

    while (!glfwWindowShouldClose(window)) {

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        mainLoop(window, &context);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (enableGraphicalSimulation) usleep(20000);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}

void mainLoop(GLFWwindow *window, Context *context) {

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

    if (!scene->camera().isTraveling()) {
        //Mise à jour en fonction de la planete actuelle
        if (currentPlanet != -1) {
            //TODO check not out of bounds
            glm::vec3 planetGraphicalPos = planets[currentPlanet].render->getPosition();
            scene->camera().moveCameraByCenterPoint(planetGraphicalPos.x, planetGraphicalPos.y, planetGraphicalPos.z);
        }

        glm::vec3 cameraUp = scene->camera().getUp();

        if (cameraUp.x == 0 && cameraUp.y == 0) {
            if (rightKey == GLFW_PRESS && leftKey != GLFW_PRESS) {
                scene->camera().rotateZ(180.0f / 70.0f);
            }
            else if (leftKey == GLFW_PRESS && rightKey != GLFW_PRESS) {
                scene->camera().rotateZ(- 180.0f / 70.0f);
            }

            if (upKey == GLFW_PRESS && downKey != GLFW_PRESS) {
                scene->camera().rotateUpDown(180.0f / 70.0f);
            }
            else if (downKey == GLFW_PRESS && upKey != GLFW_PRESS) {
                scene->camera().rotateUpDown(- 180.0f / 70.0f);
            }
        }
        else if (cameraUp.z == 0){
            //TODO contrôle de la caméra en vue de dessus
        }

    }



    //Mise à jour du monde (précision ~ 1h) t*10e6
    const double timeScale = 2;
    const double baseStep = 0.004;
    world->step(timeScale, (int) (timeScale / baseStep)); //TODO step tient compte du temps de calcul -> pour plus de précision
    // -> Sachant que les mesures effectuées sont enregistrées avec le bon temps dans le fichier.

    for (Planet &planet : planets) {
        auto &body = planet.body;

        if (body != nullptr) {
            //TODO check la présence de la planète 0
            double x = body->getX() - planets[0].body->getX();
            double y = body->getY() - planets[0].body->getY();
            double z = body->getZ() - planets[0].body->getZ();

            //Mise à jour des coordonnées de l'objet
            planet.render->setPosition((float) x, (float) y, (float) z);

            //Envoi des coordonnées dans le buffer
            std::string separator = ";";
            std::ostringstream stringstream;
            stringstream << world->getTime() << separator << x << separator << y << separator << z;
            planet.buffer.addLine(stringstream.str());

#ifdef SIMULATION_TO_COUT //TODO to parameter
            std::cout << planet.name << ":" << stringstream.str() << std::endl;
#endif
        }
    }

    if (enableGraphicalSimulation) scene->render(context);
}

//Declaration des callbacks
void glfwErrorCallback(int error, const char* description) {
    std::cerr << "There was a glfw error : " << description << std::endl;
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_KP_0) {
            currentPlanet = -1;

            auto traveling = std::make_unique<Traveling>(
                    scene->camera(), 0, 0, 30, 0, 0, 0, 0, 1, 0
            );
            traveling->setDuration(0.5f);
            scene->camera().traveling(traveling);
        }
        else if (key >= GLFW_KEY_KP_1 && key <= GLFW_KEY_KP_9){
            int targetPlanet = key - GLFW_KEY_KP_1;

            if (targetPlanet < planets.size()) {
                currentPlanet = targetPlanet;

                std::shared_ptr<Body> planetBody = planets[currentPlanet].body;

                //TODO le 15 en fonction du radius de la planète.

                auto traveling = std::make_unique<Traveling>(
                        scene->camera(),
                        planetBody->getX() + 15, planetBody->getY() + 15, planetBody->getZ() + 15,
                        planetBody->getX(), planetBody->getY(), planetBody->getZ(), 0, 0, 1
                );
                traveling->setDuration(0.5f);
                scene->camera().traveling(traveling);
            }
        }
    }
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    scene->camera().zoom((float) pow(1.2, - yoffset));
}
