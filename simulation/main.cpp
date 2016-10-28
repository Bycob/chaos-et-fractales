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
#include "physics/World.h"
#include "physics/Body.h"

//DECLARATIONS

struct Object3D {
    Object3D(std::string name, std::shared_ptr<Body> body, std::shared_ptr<Renderable> render);
    std::string name;
    std::shared_ptr<Body> body;
    std::shared_ptr<Renderable> render;
    FileBuffer buffer;
};

std::string DATE;
std::unique_ptr<Scene> scene = nullptr;
std::unique_ptr<World> world = nullptr;
std::vector<Object3D> objects;

void addObject(double mass, double x, double y, double z, double size);

/** Ajoute un objet 3D à la scene et au monde physique. */
void addObject(Object3D object);

/** Initialise la scène ainsi que le monde physique. */
void createScene();


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


//MAIN

int main(int argc, char** argv) {
    //printf("nombre : %.80Lf\n", M_PIl); //;)

    //TODO déterminer la date

    createScene();

    graphicThread();

    for (Object3D &object : objects) {
        object.buffer.writeData();
    }

    return 0;
}


//IMPLEMENTATIONS

Object3D::Object3D(std::string name, std::shared_ptr<Body> body, std::shared_ptr<Renderable> render)
    : name(name), body(body), render(render), buffer(name + "_" + DATE) {


}


void createScene() {
    world = std::make_unique<World>();
    scene = std::make_unique<Scene>();

    Light light(LIGHT_POINT, 0, 0, 0);
    scene->setLight(light);

    //Rendus
    auto moonRender = std::make_shared<RenderableSphere>(0.8, 64, 64);

    auto earthRender = std::make_shared<RenderableSphere>(0.25, 64, 64);
    earthRender->getMaterial().setDiffuse(0.5f, 0.3f, 0.1f);

    auto sunRender = std::make_shared<RenderableSphere>(2, 64, 64);
    sunRender->getMaterial().setDiffuse(0.85f, 0.75f, 0.1f);
    sunRender->getMaterial().setEmit(true);

    //Physique

    //1e10 m, 1e21 kg, tps 1e6 s -> G *= 1e3 (- 10*3 + 21 + 6*2)
    world->setGravityConstant(6.7e-11 * 1e3);

    auto moonBody = std::make_shared<Body>(73.0);
    moonBody->setPosition(-20.0, 0, 0);
    moonBody->setSpeed(0, 5e-6, 0);

    auto earthBody = std::make_shared<Body>(6000.0);
    earthBody->setPosition(-15, 0, 0);
    earthBody->setSpeed(0, 2.9, 0);

    auto sunBody = std::make_shared<Body>(2e9);

    //Ajout des objets
    //addObject({"moon", moonBody, moonRender});
    addObject({"earth", earthBody, earthRender});
    addObject({"sun", sunBody, sunRender});
}

void addObject(double mass, double x, double y, double z, double size) {
    auto sphereRender = std::make_shared<RenderableSphere>(size, 64, 64);
    sphereRender->getMaterial().setDiffuse(0.85f, 0.75f, 0.1f);
    auto sphereBody = std::make_shared<Body>(mass);
    sphereBody->setPosition(x, y, z);
    addObject({"sun", sphereBody, sphereRender});
}

void addObject(Object3D object) {
    objects.push_back(object);
    if (object.body != nullptr) world->addObject(object.body);
    if (object.render != nullptr) scene->addObject(object.render);
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
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simulation", NULL, NULL);

    if (!window) {
        std::cerr << "Erreur au lancement de glfw." << std::endl;
        return;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, glfwKeyCallback);

    //Création du contexte
    Context context(window);

    glEnable(GL_MULTISAMPLE);
    glClearColor(3 / 255.0f, 0 / 255.0f, 24 / 255.0f, 1);

    while (!glfwWindowShouldClose(window)) {

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        mainLoop(window, &context);

        glfwSwapBuffers(window);
        glfwPollEvents();

        usleep(20000);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}

void mainLoop(GLFWwindow *window, Context *context) {

    //Controles
    int rightKey = glfwGetKey(window, GLFW_KEY_RIGHT);
    int leftKey = glfwGetKey(window, GLFW_KEY_LEFT);

    if (scene->camera().isTraveling());
    else if (rightKey == GLFW_PRESS && leftKey != GLFW_PRESS) {
        scene->camera().rotateZ(180.0f / 50.0f);
    }
    else if (leftKey == GLFW_PRESS && rightKey != GLFW_PRESS) {
        scene->camera().rotateZ(- 180.0f / 50.0f);
    }

    //Mise à jour du monde (précision ~ 1h)
    world->step(0.02, 5); //TODO step tient compte du temps de calcul -> pour plus de précision
    // -> Sachant que les mesures effectuées sont enregistrées avec le bon temps dans le fichier.

    for (Object3D &object : objects) {
        auto &body = object.body;

        if (body != nullptr) {
            //Mise à jour des coordonnées de l'objet
            object.render->setPosition((float) body->getX(), (float) body->getY(), (float) body->getZ());

            //Envoi des coordonnées dans le buffer
            std::string separator = ";";
            std::ostringstream stringstream;
            stringstream << world->getTime() << separator << body->getX() << separator << body->getY() << separator << body->getZ();
            object.buffer.addLine(stringstream.str());
        }
    }

    scene->render(context);
}

//Declaration des callbacks
void glfwErrorCallback(int error, const char* description) {
    std::cerr << "There was a glfw error : " << description << std::endl;
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_KP_0 && action == GLFW_PRESS) {
        auto traveling = std::make_unique<Traveling>(
                scene->camera(), 0, 0, 30, 0, 0, 0, 0, 1, 0
        );
        traveling->setDuration(0.5f);
        scene->camera().traveling(traveling);
    }
    else if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) {
        auto traveling = std::make_unique<Traveling>(
                scene->camera(), 15, 15, 15, 0, 0, 0, 0, 0, 1
        );
        traveling->setDuration(0.5f);
        scene->camera().traveling(traveling);
    }
}
