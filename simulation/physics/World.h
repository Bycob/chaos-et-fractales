//
// Created by louis on 22/10/16.
//

#ifndef RENDUOPENGL_WORLD_H
#define RENDUOPENGL_WORLD_H

#define GRAVITY_CONSTANT 6.7e-11

#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Body;

typedef glm::tvec3<double, glm::highp> vec3d;
typedef glm::tmat3x4<double, glm::highp> mat3x4d;

enum class Method {
    EULER,
    RUNGE_KUTTA
};

class World {

public :
    World();

    virtual ~World() {};

    void setMethod(const Method & method);
    void addObject(std::shared_ptr<Body> body);

    void setGravityConstant(double g) { this->G = g;}
    double getGravityConstant() {return G;}

    double getTime() {return time;}
    vec3d getSystemLinearMomentum();
    vec3d getSystemPosition();

    ///Effectue la simulation sur le nombre de secondes passées en paramètre.
    void step(double seconds, int increment = 1);
private :
    //Différentes méthodes de calcul
    void euler(double t);

    mat3x4d rungekuttaFunc3Bodies(mat3x4d & in);
    void rungekutta3Bodies(double t);



    std::vector<std::shared_ptr<Body>> bodies;

    ///Ce champs contient la valeur de la constante universelle de gravitation.
    double G = GRAVITY_CONSTANT;

    /// Ce champ comptabilise le temps écoulé depuis la naissance du monde.
    double time;

    Method method = Method::EULER;
};


#endif //RENDUOPENGL_WORLD_H
