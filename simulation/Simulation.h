//
// Created by louis on 19/11/16.
//

#ifndef RENDUOPENGL_SIMULATION_H
#define RENDUOPENGL_SIMULATION_H

#include <memory>

#include "FileBuffer.h"

class Scene;
class World;
class Body;
class RenderableSphere;
class RenderableTrajectory;

struct Planet {
    std::string name;
    std::shared_ptr<Body> body;
    std::shared_ptr<RenderableSphere> render;
    std::shared_ptr<RenderableTrajectory> trajectory;
    std::shared_ptr<FileBuffer> buffer;

    Planet(std::string name, std::shared_ptr<Body> body, std::shared_ptr<RenderableSphere> render);
};

class Simulation {

public :
    Simulation(std::string name);
    Simulation(std::string name, std::string loadFile);

    void setRVBTrajectory();

    Scene & scene() {return *this->_scene;}
    World & world() {return *this->_world;}

    void addPlanet(Planet planet);
    Planet & getPlanet(int index);
    int getPlanetCount() {return (int) _planets.size();}

    void setCameraPlanet(int planet);
    void setTrajectoryVisibility(bool visible);

    void update(double time, bool printInfos = false);

    void writeFiles();

private :
    std::string _name;
    std::string _date;

    /// Indique, dans la simulation, quelle est la planète sur laquelle est
    /// centrée la vue actuellement. Si vaut -1, alors la vue est centrée sur
    /// 0, 0
    int _currentPlanet = 0;
    /// Indique la planète qui est au centre du système. Si == -1,
    /// alors le centre du système se trouve en 0, 0
    int _originObject = -1;
    /// L'échelle de temps, càd combien de temps passe dans la simulation (en 1e6 s)
    /// lorsqu'il s'écoule 1 seconde.
    double _timeScale = 2;
    /// Le pas de discrétisation physique de la simulation.
    double _physicalStep = 0.0001;

    /// La scène qui contient les objets à afficher (planètes, trajectoires...)
    std::unique_ptr<Scene> _scene = nullptr;
    /// Le monde qui contient les parties physiques des objets
    std::unique_ptr<World> _world = nullptr;
    /// L'ensemble des planètes de la simulation.
    std::vector<Planet> _planets;
};


#endif //RENDUOPENGL_SIMULATION_H
