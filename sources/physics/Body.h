//
// Created by louis on 22/10/16.
//

#ifndef RENDUOPENGL_BODY_H
#define RENDUOPENGL_BODY_H

class World;

class Body {

public :
    Body(double mass);

    void setPosition(double x, double y, double z);
    void setSpeed(double x, double y, double z);

    double getX() {return x;};
    double getY() {return y;};
    double getZ() {return z;};

    double distance(Body &body);

private :

    double mass;
    double x, y, z;
    double vx, vy, vz;
    double ax, ay, az;

    friend class World;
};


#endif //RENDUOPENGL_BODY_H
