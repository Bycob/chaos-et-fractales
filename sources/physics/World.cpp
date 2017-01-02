//
// Created by louis on 22/10/16.
//
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>

#include "World.h"
#include "Body.h"

World::World() : time(0) {

}

void World::setMethod(const Method &method) {
    this->method = method;
}

void World::addObject(std::shared_ptr<Body> body) {
    this->bodies.push_back(body);
}

vec3d World::getSystemLinearMomentum() {
    vec3d result(0.0, 0.0, 0.0);

	// On somme les quantités de mouvement (c'est à dire la masse multipliée
	// par la vitesse) de tous les corps.
    for (auto &body : bodies) {
        vec3d speed(body->vx, body->vy, body->vz);
        result += speed * body->mass;
    }

    return result;
}

vec3d World::getSystemPosition() {
    vec3d result(0.0, 0.0, 0.0);

	// On calcule la moyenne des positions, égale à la somme de la position de tous
	// les corps du système, divisée par le nombre de corps.
    for (auto &body : bodies) {
        vec3d position(body->x, body->y, body->z);
        result += position;
    }
	result /= bodies.size();

    return result;
}

void World::step(double seconds, int multiplier) {
    time += seconds * multiplier;

    for (int i = 0 ; i < multiplier ; i++) {
        // On applique la méthode selectionnée par l'utilisateur.
        switch (method) {
        case Method::EULER :
            euler(seconds);
            break;
        case Method::RUNGE_KUTTA :
            rungekuttaNBodies(seconds);
            break;
        }
    }
}

void World::euler(double t) {

    for (auto &body : this->bodies) {
		// Pour chaque corps on calcule l'accéleration au temps actuel
        body->ax = 0;
        body->ay = 0;
        body->az = 0;

		// L'accéleration est la somme des vecteurs force de gravitation
		// exercée par chaque corps du système sur le corps étudié.
        for (auto &body2 : this->bodies) {
            if (body != body2) {
				// On précalcule la distance entre les deux corps et le produit
				// m * g / d^3
                double length = body->distance(*body2.get());
                if (length == 0) continue;

                double k = G * body2->mass / (length * length * length);

				// On ajoute les trois composantes du vecteur force aux trois composantes
				// de l'acceleration.
                body->ax += k * (body2->x - body->x);
                body->ay += k * (body2->y - body->y);
                body->az += k * (body2->z - body->z);
            }
        }

		// On intègre la vitesse pour trouver la position finale
		// (sachant que v1 = v0 + a0 * t, avec v1 vitesse finale, v0 et a0
		// vitesse et accéleration initiales)
        body->x += body->vx * t + body->ax * t * t / 2;
        body->y += body->vy * t + body->ay * t * t / 2;
        body->z += body->vz * t + body->az * t * t / 2;

		// On intègre l'accéleration pour trouver la vitesse finale
        body->vx += body->ax * t;
        body->vy += body->ay * t;
        body->vz += body->az * t;
    }
}

Matrixdv World::rungekuttaFuncNBodies(Matrixdv &in) {
	// Matrice qui contient le résultat
    Matrixdv result(6, this->bodies.size());

	// On calcule la matrice pour chaque corps i du monde, auquel correspond
	// la ligne i de la matrice d'entrée.
    for (int i = 0 ; i < this->bodies.size() ; i++) {
		// Les trois premières colonnes de la matrice résultat contiennent la
		// vitesse du corps.
        result[i][0] = in[i][3];
        result[i][1] = in[i][4];
        result[i][2] = in[i][5];

		// Les trois colonnes suivantes contiennent son accélération, égale à la
		// somme de tous les vecteurs force de gravitation exercé par chaque corps
		// j sur le corps i.
        for (int j = 0 ; j < this->bodies.size() ; j++) {
            if (i == j) continue;

			// On récupère la distance qui sépare les deux corps
            double length = bodies[i]->distance(*bodies[j]);
			// Si la distance est trop petite on risque une division par 0
            if (length < 0.000000001) continue;

			// On ajoute le vecteur force de gravitation au résultat.
            result[i][3] = result[i][3] + G * bodies[j]->mass * (in[j][0] - in[i][0]) / (length * length * length);
            result[i][4] = result[i][4] + G * bodies[j]->mass * (in[j][1] - in[i][1]) / (length * length * length);
            result[i][5] = result[i][5] + G * bodies[j]->mass * (in[j][2] - in[i][2]) / (length * length * length);
        }
    }

    return result;
}

void World::rungekuttaNBodies(double t) {

    // On place les coordonnées et la vitesse de chaque corps dans une matrice
	// de 6 colonnes et n lignes, avec n le nombre de corps.
    Matrixdv M(6, this->bodies.size());
    for (int i = 0 ; i < this->bodies.size() ; i++) {
        auto body = this->bodies[i];
        M[i][0] = body->x;
        M[i][1] = body->y;
        M[i][2] = body->z;
        M[i][3] = body->vx;
        M[i][4] = body->vy;
        M[i][5] = body->vz;
    }

    // On applique la méthode de Runge-Kutta sur la matrice ainsi créée.
	// On calcule les matrices k1, k2, k3 et k4.
    auto k1 = rungekuttaFuncNBodies(M);
    auto M2 = M + k1 * (t / 2);
    auto k2 = rungekuttaFuncNBodies(M2);
    auto M3 = M + k2 * (t / 2);
    auto k3 = rungekuttaFuncNBodies(M3);
    auto M4 = M + k3 * t;
    auto k4 = rungekuttaFuncNBodies(M4);

    // A partir des matrices k1, k2, k3 et k4, on retrouve les positions des
	// corps au bout du temps t.
    for (int i = 0 ; i < this->bodies.size() ; i++) {
        auto & body = this->bodies[i];
        body->x += (t / 6) * (k1[i][0] + 2 * k2[i][0] + 2 * k3[i][0] + k4[i][0]);
        body->y += (t / 6) * (k1[i][1] + 2 * k2[i][1] + 2 * k3[i][1] + k4[i][1]);
        body->z += (t / 6) * (k1[i][2] + 2 * k2[i][2] + 2 * k3[i][2] + k4[i][2]);
        body->vx += (t / 6) * (k1[i][3] + 2 * k2[i][3] + 2 * k3[i][3] + k4[i][3]);
        body->vy += (t / 6) * (k1[i][4] + 2 * k2[i][4] + 2 * k3[i][4] + k4[i][4]);
        body->vz += (t / 6) * (k1[i][5] + 2 * k2[i][5] + 2 * k3[i][5] + k4[i][5]);
    }
}
