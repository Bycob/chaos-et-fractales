//
// Created by louis on 22/10/16.
//

#ifndef RENDUOPENGL_WORLD_H
#define RENDUOPENGL_WORLD_H

#define GRAVITY_CONSTANT 6.7e-11

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Matrix.h"

class Body;

typedef Matrix<double> Matrixdv;
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

	/** Définit la méthode pour calculer les nouvelles positions des corps
	lorsqu'on appelle la méthode #step(double, int) */
    void setMethod(const Method & method);
    void addObject(std::shared_ptr<Body> body);

	/** Définit la constante universelle de gravitation qui sera utilisée pour
	les calculs */
    void setGravityConstant(double g) { this->G = g;}
    double getGravityConstant() {return G;}

    double getTime() {return time;}
	/**@returns La quantité de mouvement totale du système, qui est la somme
	des quantités de mouvement de chacun des corps */
    vec3d getSystemLinearMomentum();
	/** @returns La position du système, soit la moyenne des positions de tous
	les corps. */
    vec3d getSystemPosition();

    /** Calcule les nouvelles variables du système après un certain temps.
	Cette fonction peut calculer les nouvelles variables au bout de plusieurs
	pas, il suffit pour cela de mettre un multiplier différent de 1.
	Exemple : step(0.5, 10) calcule l'état du monde au bout de 5 secondes, en
	utilisant un pas de 0.5 seconde.
	@param seconds Le pas physique, utilisé par les méthodes de Runge-Kutta
	et Euler. Le pas peut être négatif, dans ce cas le temps s'écoulera en
	sens inverse.
	@param multiplier Le nombre de pas à calculer.*/
    void step(double seconds, int multiplier);
private :
    //Différentes méthodes de calcul

	/** Calcule l'état du monde au bout du temps t, en appliquant la méthode
	d'Euler. */
    void euler(double t);

	/** Fonction utilisée pour calculer les matrices k1, k2, k3 et k4 lorsqu'on
	utilise la méthode de Runge-Kutta.*/
    Matrixdv rungekuttaFuncNBodies(Matrixdv & in);
	/** Calcule l'état du monde au bout du temps t, en appliquant la méthode
	de Runge-Kutta */
    void rungekuttaNBodies(double t);


	/// Vector contenant l'ensemble des corps du monde.
    std::vector<std::shared_ptr<Body>> bodies;

    /// Ce champ contient la valeur de la constante universelle de gravitation.
    double G = GRAVITY_CONSTANT;

    /// Ce champ comptabilise le temps écoulé depuis la naissance du monde.
    double time;

	/// La méthode utilisée pour les calculs
    Method method = Method::EULER;
};


#endif //RENDUOPENGL_WORLD_H
