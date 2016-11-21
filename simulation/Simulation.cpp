//
// Created by louis on 19/11/16.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#include "Simulation.h"
#include "utils.h"
#include "physics/World.h"
#include "physics/Body.h"
#include "rendering/Scene.h"
#include "rendering/RenderableSphere.h"
#include "rendering/RenderableTrajectory.h"

Planet::Planet(std::string name, std::shared_ptr<Body> body, std::shared_ptr<RenderableSphere> render)
        : name(name), body(body), render(render), buffer(std::make_shared<FileBuffer>("")),
          trajectory(std::make_shared<RenderableTrajectory>()) {

}




Simulation::Simulation(std::string name) : _name(name),
                                           _scene(std::make_unique<Scene>()),
                                           _world(std::make_unique<World>()) {

}

Simulation::Simulation(std::string name, std::string loadFile) : Simulation(name) {

    std::ifstream file(loadFile);
    std::string contents = "";
    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            contents += line + "\n";
        }
        file.close();
    }
    else {
        throw std::runtime_error("simulation file not found : " + loadFile);
    }

    parse(contents);
}

void Simulation::parse(std::string loadedFile) {
    std::vector<std::string> lines = split(loadedFile, '\n', true);

    bool parsingStarted = false;

    for (auto &line : lines) {
        //Parametres généraux
        if (line.size() > 0 && line.at(0) == '#') {
            //On ne prend en compte la ligne que si c'est la première du fichier
            if (parsingStarted) {
                return;
            } else {
                parsingStarted = true;
                std::string paramsLine = line.substr(1);
                std::vector<std::string> params = split(paramsLine, ';', true);

                //Analyse de chaque paramètre
                for (std::string &param : params) {
                    std::vector<std::string> keyvalue = split(param, '=', true);
                    if (keyvalue.size() != 2) continue;

                    std::string key = trimSpaces(keyvalue[0]);
                    std::string value = trimSpaces(keyvalue[1]);

                    //Paramètrage de la scène en fonction du couple key, value
                    if (key == "name") {
                        //TODO vérifier le nom valide
                        _name = value;
                    } else if (key == "") {

                    }
                }
            }
        }
        //Line
        else {
            std::vector<std::string> params = split(line, ';', true);

            std::string name = "";


            for (std::string &param : params) {
                std::vector<std::string> keyvalue = split(param, '=', true);
                if (keyvalue.size() != 2) continue;

                std::string key = trimSpaces(keyvalue[0]);
                std::string value = trimSpaces(keyvalue[1]);

                //Analyse de chaque paire clé / valeur.
                if (key == "name") {
                    //TODO vérifier la conformiter du nom
                    name = value;
                }
                else if (key == "position") {

                }
            }

            if (name != "") {
                parsingStarted = true;

                auto body = std::make_shared<Body>(5);
                auto render = std::make_shared<RenderableSphere>(1, 64, 64);
                Planet toAdd(name, body, render);

                addPlanet(toAdd);
            }
        }
    }
}

void Simulation::set3BodiesSpecial() {
    if (_planets.size() < 3) return;

    _planets[0].trajectory->setColor(1, 0, 0);
    _planets[1].trajectory->setColor(0, 1, 0);
    _planets[2].trajectory->setColor(0, 0, 1);

    _planets[0].render->getMaterial().setSpecular(1, 1, 1);
    _planets[1].render->getMaterial().setSpecular(1, 1, 1);
    _planets[2].render->getMaterial().setSpecular(1, 1, 1);
}

void Simulation::addPlanet(Planet planet) {
    _planets.push_back(planet);
    if (planet.body != nullptr) {
        _world->addObject(planet.body);
    }
    if (planet.render != nullptr) {
        _scene->addObject(planet.render);
        _scene->addObject(planet.trajectory);
        planet.render->getMaterial().setSpecular(0, 0, 0);

        glm::vec3 color = randBrightColor();
        planet.trajectory->setColor(color.r, color.g, color.b);
    }
    planet.buffer->setFilename(_name + "/" + planet.name);
}

Planet& Simulation::getPlanet(int index) {
    return _planets.at(index);
}

void Simulation::setCameraPlanet(int planet) {
    if (planet > _planets.size() || planet < 0) return;

    //Utilisation de la position graphique
    glm::vec3 planetPos = _planets[planet].render->getPosition();

    //TODO le 15 en fonction du radius de la planète.

    auto traveling = std::make_unique<Traveling>(
            scene().camera(),
            planetPos.x + 15, planetPos.y + 15, planetPos.z + 15,
            planetPos.x, planetPos.y, planetPos.z, 0, 0, 1
    );
    traveling->setDuration(0.5f);
    scene().camera().traveling(traveling);
}

void Simulation::setTrajectoryVisibility(bool visible) {
    for (Planet &planet : _planets) {
        if (planet.trajectory != nullptr) {
            planet.trajectory->setActive(visible);
        }
    }
}

void Simulation::setPlanetVisibility(bool visible) {
    for (Planet &planet : _planets) {
        if (planet.render != nullptr) {
            planet.render->setActive(visible);
        }
    }
}

void Simulation::setShadowSimulation(bool shadow) {
    _isShadow = shadow;

    for (auto &planet : this->_planets) {
        if (_isShadow) {
            planet.render->getMaterial().setAlpha(SHADOW_ALPHA);
        }
        else {
            planet.render->getMaterial().setAlpha(1.0f);
        }
    }
}

void Simulation::setPaused(bool paused) {
    _pause = paused;
}

void Simulation::togglePaused() {
    setPaused(!_pause);
}

void Simulation::setReverse(bool reverse) {
    _reverse = reverse;
}

void Simulation::toggleReverse() {
    setReverse(!_reverse);
}

void Simulation::update(double time, bool printInfos) {
    if (_pause) return;
    
    //Mise à jour du monde (précision ~ 1h) t*10e6
    const double timeScale = _timeScale * time * (_reverse ? -1 : 1);
    const double baseStep = _physicalStep;
    _world->step(timeScale, (int) fabs(timeScale / baseStep));

    vec3 speed = _world->getSystemLinearMomentum();
    printf("%.20f %.20f %.20f\n", speed.x, speed.y, speed.z);

    //Mise à jour des autres composants de l'application
    for (Planet &planet : _planets) {
        auto &body = planet.body;

        if (body != nullptr) {
            //Changement de repère des objets selon les paramètres
            double x = body->getX();
            double y = body->getY();
            double z = body->getZ();

            if (_originObject != -1 && _planets.size() > _originObject) {
                x -= _planets[_originObject].body->getX();
                y -= _planets[_originObject].body->getY();
                z -= _planets[_originObject].body->getZ();
            }

            //Mise à jour du rendu de l'objet
            if (planet.render != nullptr) {
                planet.render->setPosition((float) x, (float) y, (float) z);
                planet.trajectory->addPoint((float) x, (float) y, (float) z);
            }

            //Envoi des coordonnées dans le fichier
            std::string separator = ";";
            std::ostringstream stringstream;
            stringstream << _world->getTime() << separator << x << separator << y << separator << z;
            planet.buffer->addLine(stringstream.str());

            //Envoie des coordonnées sur la sortie stdout
            if (printInfos) {
                std::cout << planet.name << ":" << stringstream.str() << std::endl;
            }
        }
    }
}

void Simulation::writeFiles() {
    createDirectory(_name);

    for (Planet &object : _planets) {
        object.buffer->writeData();
    }
}
