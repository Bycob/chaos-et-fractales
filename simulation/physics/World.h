//
// Created by louis on 22/10/16.
//

#ifndef RENDUOPENGL_WORLD_H
#define RENDUOPENGL_WORLD_H

#include <vector>
#include <memory>

class Body;

class World {

public :
    World();

    virtual ~World() {};

    void addObject(std::shared_ptr<Body> body);

    void setGravityConstant(double g) { this->G = g;}
    double getGravityConstant() {return G;}

    double getTime() {return time;}

    ///Effectue la simulation sur le nombre de secondes passées en paramètre.
    void step(double seconds, int increment = 1);
private :
    std::vector<std::shared_ptr<Body>> bodies;

    ///Ce champs contient la valeur de la constante universelle de gravitation.
    double G = 6.7e-11;

    /// Ce champ comptabilise le temps écoulé depuis la naissance du monde.
    double time;
};


#endif //RENDUOPENGL_WORLD_H
