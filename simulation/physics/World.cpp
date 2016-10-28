//
// Created by louis on 22/10/16.
//
#include <math.h>
#include <glm/glm.hpp>

#include "World.h"
#include "Body.h"

World::World() : time(0) {

}

void World::addObject(std::shared_ptr<Body> body) {
    this->bodies.push_back(body);
}

void World::step(double seconds, int increment) {
    time += seconds;

    double timeUnit = seconds / increment;
    double threshold = timeUnit * 1.5;
    bool simulation = true;

    while (simulation) {
        //Calcul du pas
        if (seconds < threshold) {
            timeUnit = seconds;
            simulation = false;
        }
        else {
            seconds -= timeUnit;
        }

        //CALCUL DES NOUVELLES VARIABLES POUR TOUS LES CORPS

        for (auto &body : this->bodies) {
            body->ax = 0;
            body->ay = 0;
            body->az = 0;

            for (auto &body2 : this->bodies) {
                if (body != body2) {
                    double length = body->distance(*body2.get());

                    body->ax += G * body2->mass * (body2->x - body->x) / (length * length * length);
                    body->ay += G * body2->mass * (body2->y - body->y) / (length * length * length);
                    body->az += G * body2->mass * (body2->z - body->z) / (length * length * length);
                }
            }

            body->vx += body->ax * timeUnit;
            body->vy += body->ay * timeUnit;
            body->vz += body->az * timeUnit;

            body->x += body->vx * timeUnit;
            body->y += body->vy * timeUnit;
            body->z += body->vz * timeUnit;
        }
    }
}