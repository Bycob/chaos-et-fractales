//
// Created by louis on 19/11/16.
//

#ifndef RENDUOPENGL_SIMULATION_H
#define RENDUOPENGL_SIMULATION_H

#include <memory>

#include "SpeedIndicator.h"
#include "FileBuffer.h"

#define SHADOW_ALPHA 0.2f

#define DEFAULT_PHYSICAL_STEP 0.0001
#define BASE_TIME_SCALE 0.1
#define COMMON_RATIO 3.1622776601683795227870632515987381339073 //sqrt(10)
#define MAX_MULTIPLIER 10
#define MIN_MULTIPLIER 0

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

    std::string getFilename() {return _filename;}
    void set3BodiesSpecial();

    Scene & scene() {return *this->_scene;}
    World & world() {return *this->_world;}

    void addPlanet(Planet planet);
    Planet & getPlanet(int index);
    int getPlanetCount() {return (int) _planets.size();}


    //Contrôles du rendu

    /** Ajoute un traveling à la caméra vers la planète dont l'ID
     * est passé en paramètres.
     * @param planet l'ID de la planète sur laquelle faire le focus.
     * Si aucune planète de la simulation ne porte cet ID, alors
     * il ne se passe rien. */
    void setCameraPlanet(int planet);
    /** Détermine s'il faut ou non afficher les trajectoires.
     * Cette méthode applique ses effets sur les simulations
     * enfant.
     * @param visible si <tt>true</tt> alors les trajectoires
     * deviennent visibles, sinon leur affichage est désactivé.*/
    void setTrajectoryVisibility(bool visible);
    /** Remet la trajectoire des planètes à 0. Autrement dit supprime
     * tous les points enregistrés sur toutes les trajectoires.
     * Cette méthode applique ses effets sur les simulations
     * enfant.*/
    void resetTrajectories();
    /** Détermine s'il faut ou non afficher les planètes.
     * Cette méthode applique ses effets sur les simulations
     * enfant.
     * @param visible si <tt>true</tt> alors les planètes
     * deviennent visibles, sinon leur affichage est désactivé.  */
    void setPlanetVisibility(bool visible);
    /** Définit le caractère "shadow" de la simulation, c'est
     * à dire si elle est affichée en transparence ou non.
     * @param shadow si <tt>true</tt> alors la simulation est
     * affichée en transparence, sinon elle ne l'est pas.*/
    void setShadowSimulation(bool shadow);


    //Contrôle de l'execution

    void setTimeMultiplier(int multiplier);
    void incrementTimeMultiplier();
    void decrementTimeMultiplier();
    int getTimeMultiplier() {
        return _timeMultiplier;
    }
    void setPaused(bool paused);
    void togglePaused();
    bool isPaused() {
        return _pause;
    }
    void setReverse(bool reverse);
    void toggleReverse();
    bool isReverse() {
        return _reverse;
    }
    void update(double time, bool printInfos = false);

    // Données

    void writeFiles();

private :
    /// Constructeur appelé pour créer des simulations
    /// asservies (enfant).
    Simulation(const Simulation * parent);

    void parse(std::string loadedFile);

    /// Si la simulation a été créée à partir d'un fichier, contient
    /// le nom de ce fichier.
    std::string _filename;

    std::string _name;
    std::string _date;

    /// Indique si cette simulation est en <i>shadow-mode</i>, c'est à dire
    /// que les planètes sont transparente pour pouvoir comparer avec une même
    /// simulation au même endroit.
    bool _isShadow = false;
    /// Indique, dans la simulation, quelle est la planète sur laquelle est
    /// centrée la vue actuellement. Si vaut -1, alors la vue est centrée sur
    /// 0, 0
    int _currentPlanet = 0;
    /// Indique la planète qui est au centre du système. Si == -1,
    /// alors le centre du système se trouve en 0, 0
    int _originObject = -1;
    /// Indique si la simulation est arrêtée
    bool _pause = false;
    /// Indique si le temps de la simulation s'écoule à l'envers
    bool _reverse = false;

    /// Le multiplieur de temps, incrémentable ou décrémentable
    /// _timeScale = BASE_TIME_SCALE * COMMON_RATIO ^ _timeMultiplier
    int _timeMultiplier;
    /// L'échelle de temps, càd combien de temps passe dans la simulation (en 1e6 s)
    /// lorsqu'il s'écoule 1 seconde.
    double _timeScale;
    /// Le pas de discrétisation physique de la simulation.
    double _physicalStep = DEFAULT_PHYSICAL_STEP;

    const Simulation * _parent;
    /// Les simulation enfant à cette simulation. Les enfants sont esclaves de
    /// la simulation parente et lui appartiennent à jamais
    std::vector<std::unique_ptr<Simulation>> _children;
    /// La scène qui contient les objets à afficher (planètes, trajectoires...)
    std::shared_ptr<Scene> _scene = nullptr;
    /// Le monde qui contient les parties physiques des objets
    std::unique_ptr<World> _world = nullptr;
    /// L'ensemble des planètes de la simulation.
    std::vector<Planet> _planets;
    /// L'indicateur graphique de vitesse.
    std::shared_ptr<SpeedIndicator> _speedIndicator;
};


#endif //RENDUOPENGL_SIMULATION_H
