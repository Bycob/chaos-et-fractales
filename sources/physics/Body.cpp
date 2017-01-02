//
// Created by louis on 22/10/16.
//

#include <math.h>

#include "Body.h"

Body::Body(double mass) : mass(mass) {

}

void Body::setPosition(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Body::setSpeed(double x, double y, double z) {
    this->vx = x;
    this->vy = y;
    this->vz = z;
}

double Body::distance(Body &body) {
    return sqrt((x - body.x) * (x - body.x) + (y - body.y) * (y - body.y) + (z - body.z) * (z - body.z));
}